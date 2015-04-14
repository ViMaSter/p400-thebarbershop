// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSCharacter.h"
#include "EngineUtils.h"

ATBSCharacter::ATBSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerCamera"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->TargetArmLength = 100.f;
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f,0.f);
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	CameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	static ConstructorHelpers::FClassFinder<ATBSRazor> ToolBP(TEXT("/Game/TheBarberShop/Assets/Tool_BP"));
	ToolClass = ToolBP.Class;

	static ConstructorHelpers::FClassFinder<ATBSCustomer> CustomerBP(TEXT("/Game/TheBarberShop/Assets/Customer_BP"));
	CustomerClass = CustomerBP.Class;

	HorizontalCameraRotationBorder = 75;
	VerticalUpperCameraRotationBorder = -35;
	VerticalLowerCameraRotationBorder = 10;

	CurrentLevel = 1;
	CurrentExperience = 0;
	CurrentExperienceToLvl = 0;

	TimeLimit = 15.f;
}


void ATBSCharacter::BeginPlay(){
	Super::BeginPlay();
	UWorld *World = GetWorld();
	if (World && CurrentCustomer == NULL){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		CurrentCustomer = World->SpawnActor<ATBSCustomer>(CustomerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	// Spawn Tool
	if (World){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.bNoCollisionFail = true;

		FVector SpawnLocation = { 0, 0, 0 };
		FRotator SpawnRotation = { 180, 0, 0 };
		Tool = World->SpawnActor<ATBSRazor>(ToolClass, SpawnLocation, SpawnRotation, SpawnParams);
	}

	// Load Level Up Data
	const FLevelUpData *CurrentData;
	const FString String;
	if (LevelData){
		FName NameCurrentLevel = FName(*(FString::FromInt(CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData>(NameCurrentLevel, String);
		if (CurrentData){
			CurrentExperienceToLvl = CurrentData->XPtoLvl;
		}
	}

	if (CurrentCustomer) LoadNewCustomer();
}

void ATBSCharacter::FinishCurrentCustomer(){
	float Result = 0.f;
	// NOT FINAL!! NEED HARD REWORK
	if (CurrentCustomer && CurrentCustomer->Beard){
		TArray<UActorComponent*> Components;
		int32 NumberShaved = 0;
		int32 NumberTrimmed = 0;
		int32 NumberNormal = 0;
		Components = CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
		for (int32 i = 0; i < Components.Num(); i++){
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*) Components[i];
			if (!Mesh->IsVisible()){
				NumberShaved++;
			}
			else if (Mesh->GetCollisionResponseToChannel(ECC_Vehicle) == ECR_Ignore){
				NumberTrimmed++;
			}
			else{
				NumberNormal++;
			}
		}
		Result = ((float)NumberShaved / Components.Num()) * 100;
		
	}
	
	GetWorldTimerManager().ClearTimer(TimerHandle);
	IncreaseEXP(50);
	UE_LOG(LogClass, Log, TEXT("*** Customer Finished with %.1f %% accuracy ***"), Result);
	LoadNewCustomer();
}

void ATBSCharacter::LoadNewCustomer(){
	// Set Timer
	GetWorldTimerManager().ClearTimer(TimerHandle);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATBSCharacter::FinishCurrentCustomer, TimeLimit, false, -1.f);

	// Create New Customer
	if (CurrentCustomer){
		((ATBSCustomer*)CurrentCustomer)->CreateNewCustomer();
	}
	else UE_LOG(LogClass, Warning, TEXT("*** No Customer_BP set in Character ***"));
}

float ATBSCharacter::GetTimeLeft(){
	if (GetWorldTimerManager().IsTimerActive(TimerHandle)){
		float TimeLeft;
		TimeLeft = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
		UE_LOG(LogClass, Log, TEXT("*** Time Left: %.2f from %.2f ***"), TimeLeft, TimeLimit);
		return TimeLeft;
	}
	return -1.f;
}


void ATBSCharacter::IncreaseEXP(int32 Value){
	const FLevelUpData *CurrentData;
	const FString String;
	if (LevelData){
		FName NameCurrentLevel = FName(*(FString::FromInt(CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData>(NameCurrentLevel, String);
		if (CurrentData){
			CurrentExperience += Value;
			if (CurrentExperience >= CurrentData->XPtoLvl){
				CurrentLevel++;
				CurrentExperience -= CurrentData->XPtoLvl;
			}
		}
	}
	
}

void ATBSCharacter::SwitchTool(bool IsNextTool){
	if (CurrentCustomer && CurrentCustomer->Beard){
		TArray<UActorComponent*> Components;
		Components = CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
		for (int32 i = 0; i < Components.Num(); i++){
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
			Mesh->AddRelativeLocation(FVector(1, 1, 1));
		}

	}
	if (Tool){
		uint8 tmp;
		if (IsNextTool)	tmp = ((uint8)Tool->ToolType + 1U) % 4;
		else tmp = ((uint8)Tool->ToolType - 1U) % 4;
		Tool->SwitchRazorTypeTo((TEnumAsByte<ETBSRazor::Type>)tmp);
	}
}

