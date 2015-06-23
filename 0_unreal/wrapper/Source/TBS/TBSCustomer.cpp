// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSCustomer.h"
#include "TBSCharacter.h"
#include "TBSPlayerController.h"


// Sets default values
ATBSCustomer::ATBSCustomer (const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	}

	CustomerStartPosition = GetActorLocation();
}

void ATBSCustomer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	LiftPositionUpdate(DeltaTime);
	LiftPositionApply();
}

void ATBSCustomer::CreateNewCustomer (int32 CharacterLevel) {
	// Reset the beard to Trimmed 0 and visibile.

	// Deprecated!
	// CreateRandomDesiredBeard(CharacterLevel);

	FindDesiredBeardFromPool(CharacterLevel);

	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
}

void ATBSCustomer::SpawnBeardPart(){
	if (Beard != NULL) {
		if (HairIndex < HairsCutted.Num()-1 && HairsCutted[HairIndex]){
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*)HairsCutted[HairIndex];
			Mesh->SetVisibility(true);
			Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
			((ATBSCharacter*)GetOwner())->Tool->Trimmed(0, HairsCutted[HairIndex]);
			HairIndex++;
		}
		else{
			if (GetWorldTimerManager().TimerExists(SpawnTimerHandle)) {
				GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
			}
			HairsCutted.Empty();
			HairIndex = 0;
		}
	}
}

void ATBSCustomer::FinisheBeardSpawning(){
	if (Beard != NULL) {
		if (HairIndex <= HairsCutted.Num() - 1 && HairsCutted[HairIndex]) {
			for (int i = HairIndex; i < HairsCutted.Num(); i++) {
				UStaticMeshComponent* Mesh = (UStaticMeshComponent*)HairsCutted[i];
				Mesh->SetVisibility(true);
				Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
				((ATBSCharacter*)GetOwner())->Tool->Trimmed(0, HairsCutted[i]);
			}
		}
		HairsCutted.Empty();
		HairIndex = 0;
	}
}

// THIS IS DEPRECATED!
// Might become handy in MS2 when tweaking some stuff
// Dont delete please
void ATBSCustomer::CreateRandomDesiredBeard(int32 MaxLevelBeard){
	ATBSCharacter* Character = (ATBSCharacter*) GetOwner();
	ATBSPlayerController* PlayerController = NULL;
	if (Character) {
		PlayerController = (ATBSPlayerController*) Character->GetController();
		
		if (PlayerController) {
			TArray<FName> PossibleBeardNames;
			TArray<FBeardNameLevelData> Data = PlayerController->GetBeardNameLevelData();
			for (int32 i = 0; i < Data.Num(); i++) {
				if (Data[i].BeardLevel <= MaxLevelBeard) {
					PossibleBeardNames.Add(Data[i].BeardName);
				}
			}
			if (PossibleBeardNames.Num() > 0) {
				int32 randomIndex;
				randomIndex = FMath::RandRange(0, PossibleBeardNames.Num()-1);
				DesiredBeard = PossibleBeardNames[randomIndex];
			}
			else {
				UE_LOG(LogClass, Warning, TEXT("*** No possible beard in data for level ***"));
				UE_LOG(LogClass, Warning, TEXT("*** Could not generate desired beard for Customer! ***"));
			}
		}
	}
}

void ATBSCustomer::FindDesiredBeardFromPool(int32 Playerlevel){
	ATBSCharacter* Character = (ATBSCharacter*)GetOwner();
	ATBSPlayerController* PlayerController = NULL;
	if (Character && Character->BeardPoolData) {
		PlayerController = (ATBSPlayerController*)Character->GetController();
		if (PlayerController) {
			TArray<FBeardNameLevelData> Data = PlayerController->GetBeardNameLevelData();

			const FString Context;
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
				}
			}
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** Could not find Character or BeardPoolData! ***"));
		}
	}
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
