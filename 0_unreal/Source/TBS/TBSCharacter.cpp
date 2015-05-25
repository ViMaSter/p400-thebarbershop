// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSCharacter.h"
#include "TBSPlayerController.h"

ATBSCharacter::ATBSCharacter (const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer) {
	RootComponent = CreateDefaultSubobject<USceneComponent> (TEXT ("Root"));

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent> (TEXT ("PlayerCamera"));
	CameraBoom->AttachTo (RootComponent);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->TargetArmLength = 100.f;
	CameraBoom->RelativeRotation = FRotator (0.f, 180.f, 0.f);
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent> (TEXT ("Cam"));
	CameraComponent->AttachTo (CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	ToolResetPosition = CreateDefaultSubobject<USceneComponent> (TEXT ("ToolResetPosition"));
	ToolResetPosition->AttachTo (CameraComponent);
	ToolResetPosition->RelativeRotation = FRotator (-59.6, 156.8, -168.0);
	ToolResetPosition->RelativeLocation = FVector (33.1f, 16.6f, 12.0f);

	static ConstructorHelpers::FClassFinder<ATBSRazor> ToolBP (TEXT ("/Game/TheBarberShop/Assets/Tool_BP"));
	ToolClass = ToolBP.Class;

	static ConstructorHelpers::FClassFinder<ATBSCustomer> CustomerBP (TEXT ("/Game/TheBarberShop/Assets/Customer_BP"));
	CustomerClass = CustomerBP.Class;

	CameraRotationLerpIntensity = 1.0f;
	HorizontalCameraRotationBorder = 75;
	VerticalUpperCameraRotationBorder = -35;
	VerticalLowerCameraRotationBorder = 10;

	CurrentLevel = 0;
	CurrentExperience = 0;
	CurrentExperienceToLvl = 0;
	CurrentCash = 0;

	TimeLimit = 99999.f;		// Hack until final Bonustimerdecision is made ~ 27.7h per customer until the Clock in UI is broken :D
}


void ATBSCharacter::BeginPlay () {
	Super::BeginPlay ();
	UWorld *World = GetWorld ();
	if (World && CurrentCustomer == NULL) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		CurrentCustomer = World->SpawnActor<ATBSCustomer> (CustomerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentCustomer) {
			CurrentCustomer->CreateNewCustomer(CurrentLevel);
		}
	}

	// Spawn Tool
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner ();
		SpawnParams.Instigator = GetInstigator ();
		SpawnParams.bNoCollisionFail = true;

		FVector SpawnLocation = {0, 0, 0};
		FRotator SpawnRotation = {180, 0, 0};
		Tool = World->SpawnActor<ATBSRazor> (ToolClass, SpawnLocation, SpawnRotation, SpawnParams);
	}

	// Load Level Up Data
	const FLevelUpData *CurrentData;
	const FString String;
	if (LevelData) {
		FName NameCurrentLevel = FName (*(FString::FromInt (CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData> (NameCurrentLevel, String);
		if (CurrentData) {
			CurrentExperienceToLvl = CurrentData->XPtoLvl;
		}
	}
	GetWorldTimerManager().ClearTimer(TimerHandle);
	GetWorldTimerManager().SetTimer(TimerHandle, TimeLimit, false, -1.f);
}

void ATBSCharacter::FinishCurrentCustomer () {
	float Result = CalculateResult();
	int32 EXP = (int32)Result;
	IncreaseEXP(EXP);
	IncreaseCash(Result);
	CalculateBonusCash();
	GetWorldTimerManager().PauseTimer(TimerHandle);
}

void ATBSCharacter::LoadNewCustomer () {
	// Create New Customer
	if (CurrentCustomer) {
		// Set Timer
		GetWorldTimerManager().ClearTimer(TimerHandle);
		GetWorldTimerManager().SetTimer(TimerHandle, TimeLimit, false, -1.f);

		((ATBSCustomer*) CurrentCustomer)->CreateNewCustomer(CurrentLevel);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** No Customer Reference! ***"));
	}
}

// Returns the remaining time of the current customer
// Returns -1.f if timer is inactive
float ATBSCharacter::GetTimeLeft() {
	if (GetWorldTimerManager().IsTimerActive(TimerHandle)) {
		float TimeLeft;
		TimeLeft = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
		//UE_LOG (LogClass, Log, TEXT ("*** Time Left: %.2f from %.2f ***"), TimeLeft, TimeLimit);
		return TimeLeft;
	}
	return -1.f;
}

float ATBSCharacter::GetTimeElapsed() {
	if (GetWorldTimerManager().IsTimerActive(TimerHandle)) {
		float TimeElapsed;
		TimeElapsed = GetWorldTimerManager().GetTimerElapsed(TimerHandle);
		//UE_LOG (LogClass, Log, TEXT ("*** Time Left: %.2f from %.2f ***"), TimeLeft, TimeLimit);
		return TimeElapsed;
	}
	return -1.f;
}

void ATBSCharacter::PauseGameTimer() {
	if (!GetWorldTimerManager().IsTimerPaused(TimerHandle)) {
		GetWorldTimerManager().PauseTimer(TimerHandle);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** No game timer active or already paused! ***"));
	}
}

void ATBSCharacter::UnpauseGameTimer() {
	if (GetWorldTimerManager().IsTimerPaused(TimerHandle)) {
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** No game timer active or already not paused! ***"));
	}
}

void ATBSCharacter::ToggleGameTimer() {
	if (GetWorldTimerManager().IsTimerPaused(TimerHandle)) {
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
	}
	else {
		GetWorldTimerManager().PauseTimer(TimerHandle);
	}
}


void ATBSCharacter::IncreaseEXP (int32 Value) {
	const FLevelUpData *CurrentData;
	const FString String;
	if (LevelData) {
		FName NameCurrentLevel = FName (*(FString::FromInt (CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData> (NameCurrentLevel, String);
		if (CurrentData) {
			CurrentExperience += Value;
			if (CurrentExperience >= CurrentData->XPtoLvl) {
				CurrentLevel++;
				CurrentExperience -= CurrentData->XPtoLvl;
			}
		}
	}
}

void ATBSCharacter::IncreaseCash(float ComparisionResult) {
	int32 CashEarned = 0;
	if (ComparisionResult < 10.0f) {
		CashEarned = 0;
	}
	else if (ComparisionResult < 37.0f) {
		CashEarned = 10;
	}
	else if (ComparisionResult < 64.0f) {
		CashEarned = 20;
	}
	else if (ComparisionResult < 91.0f) {
		CashEarned = 30;
	}
	else {
		CashEarned = 45;
	}
	CurrentCash += CashEarned;
	UE_LOG(LogClass, Log, TEXT("*** Player earned %d $ ***"), CashEarned);
}

void ATBSCharacter::SwitchTool (bool IsNextTool) {
	if (Tool) {
		uint8 CurrentTool = Tool->ToolType;
		CurrentTool += IsNextTool ? 1 : -1;
		CurrentTool = (4 + CurrentTool) % 4;
		Tool->SwitchRazorTypeTo ((ETBSRazor::Type)CurrentTool);
	}
}

void ATBSCharacter::CalculateBonusCash(){
	if (BonusCashData) {
		FTimeBonusData* CurrentData;
		const FString Context;
		for (int32 i = 0; i < BonusCashData->GetRowNames().Num(); i++) {
			FName Row = BonusCashData->GetRowNames()[i];
			CurrentData = BonusCashData->FindRow<FTimeBonusData>(Row, Context, false);
			if (CurrentData && IsInTimeRange(GetTimeElapsed(),CurrentData->TimeMin, CurrentData->TimeMax)) {
				CurrentCash += CurrentData->BonusCash;
				UE_LOG(LogClass, Log, TEXT("*** Player earned %d $ as a bonus! ***"), CurrentData->BonusCash);
				return;
			}
		}
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** Could not load BonusCashData! ***"));
	}
}

// Returns the comparison Result from the shaved beard of the customer and the CSV data
// Returns -99 as a errorcode in case of file loading issues
float ATBSCharacter::CalculateResult () {
	FBeardComparisonData* CurrentData;
	FName DesiredCustomerBeard = CurrentCustomer->DesiredBeard;
	UDataTable* BeardDataTable = ((ATBSPlayerController*)GetController())->FindDataTableToName(DesiredCustomerBeard);
	const FString Context;
	if (BeardDataTable) {
		if (CurrentCustomer && CurrentCustomer->Beard) {
			TArray<UActorComponent*> Components;
			int32 Total = 0;
			int32 Correct = 0;
			Components = CurrentCustomer->Beard->GetComponentsByClass (UStaticMeshComponent::StaticClass ());
			for (int32 i = 0; i < Components.Num (); i++) {
				int32 ComponentStatus;
				UStaticMeshComponent* Mesh = (UStaticMeshComponent*) Components[i];
				if (!Mesh->IsVisible ()) {
					ComponentStatus = 0;
				}
				else if (Mesh->GetCollisionResponseToChannel (ECC_Vehicle) == ECR_Ignore) {
					ComponentStatus = 1;
				}
				else {
					ComponentStatus = 2;
				}
				FString String = FString::FromInt (i);
				FName Row = FName (*String);
				CurrentData = BeardDataTable->FindRow<FBeardComparisonData>(Row, Context, false);

				if (CurrentData && CurrentData->HairState == ComponentStatus) Correct++;
				Total++;
			}

			float Result = ((float) Correct / (float) Total) * 100;
			UE_LOG (LogClass, Log, TEXT ("*** Customer Finished with %.1f %% accuracy ***"), Result);
			return Result;
		}
		else UE_LOG (LogClass, Warning, TEXT ("*** Could not load Beard Comparison Data! ***"));
	}
	return -99;
}

FName ATBSCharacter::GetDesiredCustomerBeard(){
	if (CurrentCustomer) {
		return CurrentCustomer->DesiredBeard;
	}
	return "DEFAULT";
}

bool ATBSCharacter::IsInTimeRange(float Time, int32 MinTime, int32 MaxTime){
	if (Time >= MinTime && Time <= MaxTime) {
		return true;
	}
	return false;
}