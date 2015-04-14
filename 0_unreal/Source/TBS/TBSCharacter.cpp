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

	static ConstructorHelpers::FClassFinder<ATBSRazor> RazorBP(TEXT("/Game/TheBarberShop/Assets/Tool_BP"));
	RazorClass = RazorBP.Class;

	HorizontalCameraRotationBorder = 75;
	VerticalUpperCameraRotationBorder = -35;
	VerticalLowerCameraRotationBorder = 10;

	CurrentLevel = 1;
	CurrentExperience = 0;
	CurrentExperienceToLvl = 0;

	TimeLimit = 5.f;
}


void ATBSCharacter::BeginPlay(){
	Super::BeginPlay();
	UWorld *World = GetWorld();

	// Set Customer  FINAL BUILD VIA REF BP WITHOUT SCENE SCANNING
	for (TActorIterator<ATBSCustomer> ActorItr(GetWorld()); ActorItr; ++ActorItr){
		CurrentCustomer =  *ActorItr;
	}

	if (CurrentCustomer) LoadNewCustomer();

	// Spawn Tool
	if (World){
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.bNoCollisionFail = true;

		FVector SpawnLocation = { 0, 0, 0 };
		FRotator SpawnRotation = { 180, 0, 0 };
		Razor = World->SpawnActor<ATBSRazor>(RazorClass, SpawnLocation, SpawnRotation, SpawnParams);
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

}

void ATBSCharacter::FinishCurrentCustomer(){
	GetWorldTimerManager().ClearTimer(TimerHandle);
	IncreaseEXP(50);
	UE_LOG(LogClass, Log, TEXT("*** Customer Finished ***"));
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
	if (Razor){
		uint8 tmp;
		if (IsNextTool)	tmp = ((uint8)Razor->RazorType + 1U) % 4;
		else tmp = ((uint8)Razor->RazorType - 1U) % 4;
		Razor->SwitchRazorTypeTo((TEnumAsByte<ETBSRazor::Type>)tmp);
	}
}

