// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSCharacter.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"
#include "TBSDataLoadWorker.h"

ATBSCharacter::ATBSCharacter (const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer) {
	RootComponent = CreateDefaultSubobject<USceneComponent> (TEXT ("Root"));

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent> (TEXT ("PlayerCamera"));
	CameraBoom->AttachTo (RootComponent);
	CameraBoom->bAbsoluteRotation = false;
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
	
	CameraRotationLerpIntensity = 1.0f;
	HorizontalCameraRotationBorder = 75;
	VerticalUpperCameraRotationBorder = -35;
	VerticalLowerCameraRotationBorder = 10;

	CurrentLevel = 0;
	CurrentExperience = 0;
	CurrentExperienceToLvl = 0;
	CurrentCash = 0;

	TimeLimit = 99999.f;		// Hack until final Bonustimerdecision is made ~ 27.7h per customer until the Clock in UI is broken :D

	PrimaryActorTick.bCanEverTick = true;
}

void ATBSCharacter::Tick(float DeltaTime){
	// Handel Loading of Comparision Data
	if (CompLoadingStarted_MT && GetWorldTimerManager().TimerExists(CompLoadingTimeHandle_MT)) {
		if (FTBSDataLoadWorker::IsThreadFinished()) {
			FTBSDataLoadWorker::Shutdown();
			CompLoadingStarted_MT = false;
			GetWorldTimerManager().ClearTimer(CompLoadingTimeHandle_MT);
		}
	}

	// Handel Loading of BonusCash Data
	if (BonusLoadingStarted_MT && GetWorldTimerManager().TimerExists(BonusLoadingTimeHandle_MT)) {
		if (FTBSDataLoadWorker::IsThreadFinished()) {
			FTBSDataLoadWorker::Shutdown();
			BonusLoadingStarted_MT = false;
			GetWorldTimerManager().ClearTimer(BonusLoadingTimeHandle_MT);
		}
	}

	// Handel Loading of Equipment Data
	if (EquipmentLodingStarted_MT && GetWorldTimerManager().TimerExists(EquipmentLoadingTimeHandle_MT)) {
		if (FTBSDataLoadWorker::IsThreadFinished()) {
			FTBSDataLoadWorker::Shutdown();
			CompLoadingStarted_MT = false;
			GetWorldTimerManager().ClearTimer(EquipmentLoadingTimeHandle_MT);
		}
	}

	// Handel Loading of Level Data
	if (LevelLodingStarted_MT && GetWorldTimerManager().TimerExists(LevelLoadingTimeHandle_MT)) {
		if (FTBSDataLoadWorker::IsThreadFinished()) {
			FTBSDataLoadWorker::Shutdown();
			LevelLodingStarted_MT = false;
			GetWorldTimerManager().ClearTimer(LevelLoadingTimeHandle_MT);
		}
	}
}

void ATBSCharacter::BeginPlay () {
	Super::BeginPlay ();
	UWorld *World = GetWorld ();
	if (World && FirstCustomer == NULL) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		FirstCustomer = World->SpawnActor<ATBSCustomer>(CustomerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	if (World && SecondCustomer == NULL) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoCollisionFail = true;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		SecondCustomer = World->SpawnActor<ATBSCustomer>(CustomerClass, FVector(0, -810, 0), FRotator::ZeroRotator, SpawnParams);
	}
	if (FirstCustomer) {
		CurrentCustomer = FirstCustomer;
	}
	if (SecondCustomer){
		NextCustomer = SecondCustomer;
	}

	// Spawn Tool
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner ();
		SpawnParams.Instigator = GetInstigator ();
		SpawnParams.bNoCollisionFail = true;

		FVector SpawnLocation = {0, 0, 0};
		FRotator SpawnRotation = {180, 0, 0};
		Tool = World->SpawnActor<ATBSRazor>(ToolClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
	if (Controller) {
		((ATBSPlayerController*)Controller)->PlayerCharacter = this;
	}

}

void ATBSCharacter::StartGame() {
	GameIsRunning = true;
	CurrentCustomer->CreateNewCustomer(CurrentLevel);
	CurrentBeardRow = ((ATBSPlayerController*)GetController())->FindDataRowToName(CurrentCustomer->DesiredBeard);

	// Load Level Up Data
	const FLevelUpData *CurrentData;
	const FString String;
	if (LevelData) {
		FName NameCurrentLevel = FName(*(FString::FromInt(CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData>(NameCurrentLevel, String);
		if (CurrentData) {
			CurrentExperienceToLvl = CurrentData->XPtoLvl;
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle, TimeLimit, false, -1.f);

	// Load Data with another thread
	if (CurrentCustomer) {
		FName DesiredCustomerBeard = CurrentCustomer->DesiredBeard;
		UDataTable* BeardDataTable = ((ATBSPlayerController*)GetController())->FindDataTableToName(DesiredCustomerBeard);
		if (BeardDataTable) {
			FTBSDataLoadWorker::JoyInitBeardComp(BeardData_MT, BeardDataTable);
			GetWorldTimerManager().SetTimer(CompLoadingTimeHandle_MT, 0.1f, true, -1.f);
			CompLoadingStarted_MT = true;
		}
	}

	if (EquipmentData) {
		FTBSDataLoadWorker::JoyInitEquipment(EquipmentData_MT, EquipmentData);
		GetWorldTimerManager().SetTimer(EquipmentLoadingTimeHandle_MT, 0.1f, true, -1.f);
		EquipmentLodingStarted_MT = true;
	}

	if (LevelData) {
		FTBSDataLoadWorker::JoyInitLevel(LevelData_MT, LevelData);
		GetWorldTimerManager().SetTimer(LevelLoadingTimeHandle_MT, 0.1f, true, -1.f);
		LevelLodingStarted_MT = true;
	}

	if (BonusCashData) {
		FTBSDataLoadWorker::JoyInitBonus(TimeBonusData_MT, BonusCashData);
		GetWorldTimerManager().SetTimer(BonusLoadingTimeHandle_MT, 0.1f, true, -1.f);
		LevelLodingStarted_MT = true;
	}

	CurrentCustomer = FirstCustomer;
	NextCustomer = SecondCustomer;
	FirstCustomerActive = true;

	CurrentCustomer->IsCurrentCustomer = true;
	CurrentCustomer->SetActorHiddenInGame(false);
	CurrentCustomer->Beard->SetActorHiddenInGame(false);

	NextCustomer->IsCurrentCustomer = false;
	NextCustomer->SetActorHiddenInGame(true);
	NextCustomer->Beard->SetActorHiddenInGame(true);

	if (GetController()) {
		((ATBSPlayerController*)GetController())->ResetCamera();
	}

	Tool->SwitchRazorTypeTo(ETBSRazor::TBSRazorBig);

	SetActorLocation(FVector(0, 0, 340));

	BeardResult = 0;
	LastBeardResult = 0;
}

void ATBSCharacter::FinishCurrentCustomer () {
	BeardResult = CalculateResult();
	int32 EXP = (int32)BeardResult;
	LastBeardResult = BeardResult;

	IncreaseEXP(EXP);
	IncreaseCash(BeardResult);
	CalculateBonusCash();
	SaveSessionData();

	GetWorldTimerManager().PauseTimer(TimerHandle);
	if (Controller){
		((ATBSPlayerController*)Controller)->FinishedCurrentCustomer();
	}

	// Background Load New Customer
	LoadNewCustomer();
}

void ATBSCharacter::TransitionToNewCustomer() {
	// Set Timer
	if (GetWorldTimerManager().TimerExists(TimerHandle)){
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	GetWorldTimerManager().SetTimer(TimerHandle, TimeLimit, false, -1.f);

	if (FirstCustomerActive && SecondCustomer) {
		SetActorLocation(FVector(0, -810, 340));
		CurrentCustomer = SecondCustomer;
		NextCustomer = FirstCustomer;
	}
	else if (!FirstCustomerActive && FirstCustomer) {
		SetActorLocation(FVector(0, 0, 340));
		CurrentCustomer = FirstCustomer;
		NextCustomer = SecondCustomer;
	}
	SwapedScreenCap();

	if (GetController()) {
		((ATBSPlayerController*)GetController())->ResetCamera();
	}

	CurrentCustomer->SetActorHiddenInGame(false);
	CurrentCustomer->Beard->SetActorHiddenInGame(false);
	CurrentCustomer->IsCurrentCustomer = true;

	NextCustomer->SetActorHiddenInGame(true);
	NextCustomer->Beard->SetActorHiddenInGame(true);
	NextCustomer->IsCurrentCustomer = false;

	FirstCustomerActive = !FirstCustomerActive;
	//GetWorldTimerManager().SetTimer(NextCustomer->SpawnTimerHandle, NextCustomer, &ATBSCustomer::SpawnBeardPart, 0.05f, true);
	CurrentCustomer->FinisheBeardSpawning();
}


void ATBSCharacter::LoadNewCustomer () {
	// Create New Customer
	if (NextCustomer) {
		NextCustomer->CreateNewCustomer(CurrentLevel);
		CurrentBeardRow = ((ATBSPlayerController*)GetController())->FindDataRowToName(CurrentCustomer->DesiredBeard);
		// Load Data with another thread
		if (NextCustomer) {
			FName DesiredCustomerBeard = CurrentCustomer->DesiredBeard;
			UDataTable* BeardDataTable = ((ATBSPlayerController*)GetController())->FindDataTableToName(DesiredCustomerBeard);
			if (BeardDataTable) {
				FTBSDataLoadWorker::JoyInitBeardComp(BeardData_MT, BeardDataTable);
				GetWorldTimerManager().SetTimer(CompLoadingTimeHandle_MT, 0.1f, true, -1.f);
				CompLoadingStarted_MT = true;
			}
		}

		// Setup for next Customer
		SessionID++;
		BeardResult = 0;
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
	if (GetWorldTimerManager().TimerExists(TimerHandle) && !GetWorldTimerManager().IsTimerPaused(TimerHandle)) {
		GetWorldTimerManager().PauseTimer(TimerHandle);
		GameIsRunning = false;
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** No game timer active or already paused! ***"));
	}
}

void ATBSCharacter::UnpauseGameTimer() {
	if (GetWorldTimerManager().TimerExists(TimerHandle) && GetWorldTimerManager().IsTimerPaused(TimerHandle)) {
		GetWorldTimerManager().UnPauseTimer(TimerHandle);
		GameIsRunning = true;
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** No game timer active or not paused! ***"));
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
		int32 CurrentTool = Tool->ToolType;
		CurrentTool += IsNextTool ? 1 : -1;
		CurrentTool = (3 + CurrentTool) % 3;
		Tool->SwitchRazorTypeTo ((ETBSRazor::Type)CurrentTool);
	}
}

void ATBSCharacter::CalculateBonusCash(){
	for (int32 i = 0; i < TimeBonusData_MT.Num(); i++) {
		if (TimeBonusData_MT[i] && IsInTimeRange(GetTimeElapsed(), TimeBonusData_MT[i]->TimeMin, TimeBonusData_MT[i]->TimeMax)) {
			CurrentCash += TimeBonusData_MT[i]->BonusCash;
			UE_LOG(LogClass, Log, TEXT("*** Player earned %d $ as a bonus! ***"), TimeBonusData_MT[i]->BonusCash);
			return;
		}
	}
}

// Returns the comparison Result from the shaved beard of the customer and the CSV data
float ATBSCharacter::CalculateResult () {
	if (Tool && Tool->TrimmedBeardInstances && Tool->InstancedSMComponent) {
		TArray<UActorComponent*> Components;
		int32 Total = Tool->InstancedSMComponent->GetInstanceCount();
		int32 Incorrect = 0;
		int32 NumTrimmed = Tool->TrimmedBeardInstances->GetInstanceCount();
		int32 NumShaved = Tool->CutHairsIndices.Num() - NumTrimmed;
		int32 TotalTarget = 0;
		int32 NumTargetTrimmed = 0;
		int32 NumTargetShaved = 0;

		for (int32 i = 0; i < BeardData_MT.Num(); i++){
			switch (BeardData_MT[i]->HairState)
			{
			case(0) : // Shaved
				NumTargetShaved++;
				break;
			case(1) : // Trimmed
				NumTargetTrimmed++;
				break;
			case(2) : // Unshaved
				break;
			}
			TotalTarget++;
		}
		Incorrect = NumTargetTrimmed - NumTrimmed + NumTargetShaved - NumShaved;
		float Result = ((float)(Total - Incorrect) / (float)Total) * 100;
		UE_LOG(LogClass, Log, TEXT("*** Customer Finished with %.1f %% accuracy ***"), Result);
		return Result;
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard Comparison Data! ***"));
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

void ATBSCharacter::SaveSessionData() {
	if (GetWorld()->GetGameState<ATBSGameState>()) {
		ATBSGameState* gameState = GetWorld()->GetGameState<ATBSGameState>();
		FTBSSessionState sessionState;
		sessionState.SessionID = SessionID;
		sessionState.BeardName = CurrentCustomer->DesiredBeard;
		sessionState.BeardResult = BeardResult;
		sessionState.TimeRequired = GetTimeElapsed();

		gameState->CurrentSaveGame->AddSessionState(sessionState);
		return;
	}
}

#pragma region Equipment
TArray<int32> ATBSCharacter::GetObtainedEquipment() {
	return ((ATBSGameState*)(GetWorld()->GetGameState()))->CurrentSaveGame->ObtainedEquipment;
}

bool ATBSCharacter::BuyEquipment(int32 ID) {
	FTBSEquipmentData* CurrentData;
	const FString Context;
	if (EquipmentData) {
		FString String = FString::FromInt(ID);
		FName Row = FName(*String);
		CurrentData = EquipmentData->FindRow<FTBSEquipmentData>(Row, Context, false);
		if (CurrentData && CurrentData->Cost <= CurrentCash) {
			CurrentCash -= CurrentData->Cost;
			((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Add(ID);
			EquipedItem(ID);
			return true;
		}
	}
	return false;
}

FTBSEquipmentData ATBSCharacter::GetEquipmentByID(int32 ID) {
#pragma region MS2
	const FString ContextString(TEXT("FTBSEquipmentData"));
	return *EquipmentData->FindRow<FTBSEquipmentData>(FName(*FString::FromInt(ID)), ContextString);
#pragma endregion
#pragma region Final
	//return (GetEquipmentList()[ID]);
#pragma endregion
}

TMap<int32, FTBSEquipmentData> ATBSCharacter::GetEquipmentList() {
	TMap<int32, FTBSEquipmentData> EquipmentList;
	if (EquipmentData_MT.Num() > 0) {
		for (int32 i = 0; i < EquipmentData_MT.Num(); i++) {
			FTBSEquipmentData Data;
			Data.Cost = EquipmentData_MT[i]->Cost;
			Data.Name = EquipmentData_MT[i]->Name;
			Data.EquipmentID = EquipmentData_MT[i]->EquipmentID;
			EquipmentList.Add(Data.EquipmentID, Data);
		}
	}
	return EquipmentList;
}

TArray<FTBSEquipmentData> ATBSCharacter::GetEquipmentListAsArray() {
	TArray<FTBSEquipmentData> TargetArray;
	GetEquipmentList().GenerateValueArray(TargetArray);
	return TargetArray;
}

bool ATBSCharacter::EquipItem(int32 ID) {
	if (((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Find(ID) == INDEX_NONE) {
		UE_LOG(LogClass, Warning, TEXT("*** Wrong ID or Equipment not obtained! ***"));
		return false;
	}
	else {
		EquipedItem(ID);
		return true;
	}
}

#pragma endregion Equipment