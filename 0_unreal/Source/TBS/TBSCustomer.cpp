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

	/* FClassfinder does not find the Blueprint for some reason. So it resets for some reason the Bearclass with Null.
	   This is a short fix, so the autobuilt will work properly.
	   static ConstructorHelpers::FClassFinder<AActor> BeardBP(TEXT("/Game/TheBarberShop/Assets/Characters/BeardStyles/BeardStyle_00_BP"));
	   if (BeardBP.Class != NULL){
	   BeardClass = BeardBP.Class;
	   }*/

}
// Called when the game starts or when spawned
void ATBSCustomer::BeginPlay () {
	Super::BeginPlay ();
	if (BeardClass && Beard == NULL) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		Beard = GetWorld()->SpawnActor<AActor>(BeardClass, FVector(0, 0, 340), FRotator::ZeroRotator, SpawnParams);
	}
}

void ATBSCustomer::CreateNewCustomer (int32 CharacterLevel) {
	// Reset the beard to Trimmed 0 and visibile.
	if (Beard != NULL) {
		TArray<UActorComponent*> Components;
		Components = Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
		for (int32 i = 0; i < Components.Num(); i++) {
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
			Mesh->SetVisibility(true);
			Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
			((ATBSCharacter*)GetOwner())->Tool->Trimmed(0, Components[i]);
		}
	}
	// Deprecated!
	// CreateRandomDesiredBeard(CharacterLevel);

	FindDesiredBeardFromPool(CharacterLevel);

	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
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