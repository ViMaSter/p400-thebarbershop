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

	ToolResetPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ToolResetPosition"));
	ToolResetPosition->AttachTo(CameraComponent);
	ToolResetPosition->RelativeRotation = FRotator(-59.6, 156.8, -168.0);
	ToolResetPosition->RelativeLocation = FVector(33.1f, 16.6f, 12.0f);

	static ConstructorHelpers::FClassFinder<ATBSRazor> ToolBP(TEXT("/Game/TheBarberShop/Assets/Tool_BP"));
	ToolClass = ToolBP.Class;

	static ConstructorHelpers::FClassFinder<ATBSCustomer> CustomerBP(TEXT("/Game/TheBarberShop/Assets/Customer_BP"));
	CustomerClass = CustomerBP.Class;

	CameraRotationLerpIntensity = 1.0f;
	HorizontalCameraRotationBorder = 75;
	VerticalUpperCameraRotationBorder = -35;
	VerticalLowerCameraRotationBorder = 10;

	CurrentLevel = 1;
	CurrentExperience = 0;
	CurrentExperienceToLvl = 0;

	TimeLimit = 15.f;
}


void ATBSCharacter::BeginPlay()
{
	Super::BeginPlay();
	UWorld *World = GetWorld();
	if (World && CurrentCustomer == NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		CurrentCustomer = World->SpawnActor<ATBSCustomer>(CustomerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	// Spawn Tool
	if (World)
	{
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
	if (LevelData)
	{
		FName NameCurrentLevel = FName(*(FString::FromInt(CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData>(NameCurrentLevel, String);
		if (CurrentData){
			CurrentExperienceToLvl = CurrentData->XPtoLvl;
		}
	}

	if (CurrentCustomer) LoadNewCustomer();
}

void ATBSCharacter::FinishCurrentCustomer()
{
	CalculateResult();
	IncreaseEXP(50);
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ATBSCharacter::LoadNewCustomer()
{
	// Set Timer
	GetWorldTimerManager().ClearTimer(TimerHandle);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATBSCharacter::FinishCurrentCustomer, TimeLimit, false, -1.f);

	// Create New Customer
	if (CurrentCustomer)
	{
		((ATBSCustomer*)CurrentCustomer)->CreateNewCustomer();
	}
	else UE_LOG(LogClass, Warning, TEXT("*** No Customer Reference! ***"));
}

// Returns the remaining time of the current customer
// Returns -1.f if timer is inactive
float ATBSCharacter::GetTimeLeft()
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle))
	{
		float TimeLeft;
		TimeLeft = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
		UE_LOG(LogClass, Log, TEXT("*** Time Left: %.2f from %.2f ***"), TimeLeft, TimeLimit);
		return TimeLeft;
	}
	return -1.f;
}


void ATBSCharacter::IncreaseEXP(int32 Value)
{
	const FLevelUpData *CurrentData;
	const FString String;
	if (LevelData)
	{
		FName NameCurrentLevel = FName(*(FString::FromInt(CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData>(NameCurrentLevel, String);
		if (CurrentData)
		{
			CurrentExperience += Value;
			if (CurrentExperience >= CurrentData->XPtoLvl)
			{
				CurrentLevel++;
				CurrentExperience -= CurrentData->XPtoLvl;
			}
		}
	}
}

void ATBSCharacter::SwitchTool(bool IsNextTool)
{
	if (Tool)
	{
		uint8 CurrentTool = Tool->ToolType;
		CurrentTool += IsNextTool ? 1 : -1;
		CurrentTool = (4 + CurrentTool) % 4;
		Tool->SwitchRazorTypeTo((ETBSRazor::Type)CurrentTool);
	}
}

// Returns the comparison Result from the shaved beard of the customer and the CSV data
// Returns -99 as a errorcode in case of file loading issues
float ATBSCharacter::CalculateResult(){
	FBeardComparisonData* CurrentData;
	const FString Context;
	if (BeardData[0])
	{
		if (CurrentCustomer && CurrentCustomer->Beard)
		{
			TArray<UActorComponent*> Components;
			int32 Total = 0;
			int32 Correct = 0;
			Components = CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
			for (int32 i = 0; i < Components.Num(); i++){
				int32 ComponentStatus;
				UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
				if (!Mesh->IsVisible())
				{
					ComponentStatus = 0;
				}
				else if (Mesh->GetCollisionResponseToChannel(ECC_Vehicle) == ECR_Ignore)
				{
					ComponentStatus = 1;
				}
				else
				{
					ComponentStatus = 2;
				}
				FString String = FString::FromInt(i);
				FName Row = FName(*String);
				CurrentData = BeardData[0]->FindRow<FBeardComparisonData>(Row, Context, false);

				if (CurrentData && CurrentData->HairState == ComponentStatus) Correct++;
				Total++;
			}

			float Result = ((float) Correct / (float) Total)*100;
			UE_LOG(LogClass, Log, TEXT("*** Customer Finished with %.1f %% accuracy ***"), Result);
			return Result;
		}
		else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard Comparison Data! ***"));
	}
	return -99;
}

