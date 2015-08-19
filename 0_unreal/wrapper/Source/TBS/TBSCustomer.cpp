// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSCustomer.h"
#include "TBSCharacter.h"
#include "TBSGameState.h"
#include "TBSPlayerController.h"


// Sets default values
ATBSCustomer::ATBSCustomer (const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DesiredBeard = "DEFAULT";

	// Lifting of seat
#pragma region Lift
	LiftPositionTarget = 0.0f;
	LiftPositionCurrent = LiftPositionTarget;
	LiftPositionLerpSpeed = 0.05f;

	LiftLastPressed = -1.0f;
	LiftTimeForHold = 0.2f;

	LiftUpSteps = 10.0f;
	LiftDownSpeed = 50.0f;
#pragma endregion Lift
}
// Called when the game starts or when spawned
void ATBSCustomer::BeginPlay () {
	Super::BeginPlay ();
	if (BeardClass && Beard == NULL) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		Beard = GetWorld()->SpawnActor<AActor>(BeardClass, GetActorLocation()+FVector(0,0,340), FRotator::ZeroRotator, SpawnParams);
		BeardStartPosition = Beard->GetActorLocation();
		Beard->SetActorHiddenInGame(true);
	}

	CustomerStartPosition = GetActorLocation();
}

void ATBSCustomer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	LiftPositionUpdate(DeltaTime);
	LiftPositionApply();
}

void ATBSCustomer::CreateNewCustomer (int32 CharacterLevel) {
	FindDesiredBeardFromPool(CharacterLevel);

	CustomerTimeLimit = FMath::RandRange(30, 60);
	CustomerSatisfaction = 100;

	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
}

void ATBSCustomer::FindDesiredBeardFromPool(int32 Playerlevel){
	ATBSCharacter* Character = (ATBSCharacter*)GetOwner();
	ATBSPlayerController* PlayerController = NULL;
	if (Character && Character->BeardPoolData) {
		PlayerController = (ATBSPlayerController*)Character->GetController();
		if (PlayerController) {

			// Catch beard data
			TArray<FBeardNameLevelData> Data = PlayerController->GetBeardNameLevelData();

			int32 NumberOfPlaythrough = 0;
			// Check which playthrough this is
			if (GetWorld()->GetGameState<ATBSGameState>()->CurrentSaveGame)	{
				NumberOfPlaythrough = GetWorld()->GetGameState<ATBSGameState>()->CurrentSaveGame->SessionList.Num();
			}			

			// Force a certain beard if we're still in the "tutorial phase"
			if (GetWorld()->GetGameState<ATBSGameState>()->CurrentSaveGame && !GetWorld()->GetGameState<ATBSGameState>()->CurrentSaveGame->UsedOtherTools && ForcedSessionBeards.Num() > 1) {
				DesiredBeard = "DEFAULT";
				for (int32 i = 0; i < Data.Num(); i++) {
					if (Data[i].UniqueID == ForcedSessionBeards[FMath::Min(NumberOfPlaythrough, ForcedSessionBeards.Num()-1)]) {
						DesiredBeard = Data[i].BeardName;
						PortraitTexture = Data[i].ComparisionScreenshot;
						UE_LOG(LogClass, Log, TEXT("*** Tutorial: Falling back to %s. ***"), *DesiredBeard.ToString());
						break;
					}
				}
				// Fallback, if no valid ID was set in ForcedSessionBeards 
				if (DesiredBeard == "DEFAULT") {
					if (Data.Num() > 0) {
						DesiredBeard = Data[0].BeardName;
						PortraitTexture = Data[0].ComparisionScreenshot;
						UE_LOG(LogClass, Warning, TEXT("*** No beard in BeardPoolData using UniqueIdentifier %d! Falling back to the first one available! ***"), ForcedSessionBeards[FMath::Min(NumberOfPlaythrough, ForcedSessionBeards.Num() - 1)]);
					}
				}
			}
			// Or randomly choose from our pool
			else {
				const FString Context = FString("");
				FBeardPoolData* CurrentData;

				int32 MinBeardID = -99;
				int32 MaxBeardID = -99;
				for (int32 i = 0; i < Character->BeardCollection->GetRowNames().Num(); i++) {
					FName Row = Character->BeardCollection->GetRowNames()[i];
					CurrentData = Character->BeardPoolData->FindRow<FBeardPoolData>(Row, Context, false);
					if (CurrentData) {
						if (CurrentData->PlayerLevel == Playerlevel) {
							MinBeardID = CurrentData->BeardMinNumber;
							MaxBeardID = CurrentData->BeardMaxNumber;
							break;
						}
					}
					else {
						UE_LOG(LogClass, Warning, TEXT("*** Could not find Playerlevel in BeardPoolData! ***"));
						DesiredBeard = "DEFAULT";
						return;
					}
				}

				int32 randID = FMath::RandRange(MinBeardID, MaxBeardID);

				// Random stuff
				for (int32 i = 0; i < Data.Num(); i++) {
					if (Data[i].UniqueID == randID) {
						DesiredBeard = Data[i].BeardName;
						PortraitTexture = Data[i].ComparisionScreenshot;
					}
				}
			}
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** Could not find Character or BeardPoolData! ***"));
		}
	}
}

void ATBSCustomer::HurtCustomer() {
	CustomerSatisfaction -= 10;
	TimesHurt++;
}

void ATBSCustomer::LiftPositionPressed() {
	LiftLastPressed = GetWorld()->GetTimeSeconds();
}

void ATBSCustomer::LiftPositionReleased() {
	if ((GetWorld()->GetTimeSeconds() - LiftLastPressed) <= LiftTimeForHold) {
		this->LiftPositionUp();
	}
	LiftLastPressed = -1.0f;
}

void ATBSCustomer::LiftPositionApply() {
	SetActorLocation(CustomerStartPosition + FVector(0, 0, LiftPositionCurrent));
	
	// @Notice: Feature is only "nice to have"
	// Enabling the line below does move the beard, but causes huge performance issues
	// Beard->SetActorLocation(BeardStartPosition + FVector(0, 0, LiftPositionCurrent));
}

void ATBSCustomer::LiftPositionUpdate(float DeltaTime) {
	if ((GetWorld()->GetTimeSeconds() - LiftLastPressed) > LiftTimeForHold && LiftLastPressed != -1.0f) {
		LiftPositionDown(DeltaTime);
	}

	LiftPositionTarget = FMath::Clamp(LiftPositionTarget, LiftLimits.X, LiftLimits.Y);
	LiftPositionCurrent = FMath::Lerp(LiftPositionCurrent, LiftPositionTarget, LiftPositionLerpSpeed);
}

void ATBSCustomer::LiftPositionUp() {
	float currentOffset = FMath::Fmod(LiftPositionTarget, LiftUpSteps);
	LiftPositionTarget -= currentOffset;
	LiftPositionTarget += LiftUpSteps;
}

void ATBSCustomer::LiftPositionDown(float DeltaTime) {
	LiftPositionTarget -= LiftDownSpeed * DeltaTime;
}
