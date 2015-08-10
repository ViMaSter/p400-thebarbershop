// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSCharacter.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"

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

	CurrentLevel = 1;
	CurrentExperience = 0;
	CurrentExperienceToLvl = 0;
	CurrentCash = 0;
	CashEarned = 0;
	CashPenalty = 0;

	TimeLimit = 99999.f;		// Hack until final Bonustimerdecision is made ~ 27.7h per customer until the Clock in UI is broken :D

	PrimaryActorTick.bCanEverTick = true;
}

void ATBSCharacter::Tick(float DeltaTime){
	CheckMTTasks();
}

void ATBSCharacter::CheckMTTasks() {
	int32 Rdycounter = 0;
	for (FMTTask Task : MTTasks) {
		if (Task.TaskStarted && GetWorldTimerManager().TimerExists(Task.Handle) && Task.Runnable->IsTaskFinished(Task.Type)) {
			Task.TaskStarted = false;
			GetWorldTimerManager().ClearTimer(Task.Handle);
			Rdycounter++;
			switch (Task.Type)
			{
				case ETBSMultiThreadingTask::BeardComparison:
					UE_LOG(LogClass, Log, TEXT("*** Thread: Finished loading BeardCompData ***"), (uint32)Task.Type);
					break;
				case ETBSMultiThreadingTask::Equipment:
					UE_LOG(LogClass, Log, TEXT("*** Thread: Finished loading EquipmentData ***"), (uint32)Task.Type);
					break;
				case ETBSMultiThreadingTask::Level:
					UE_LOG(LogClass, Log, TEXT("*** Thread: Finished loading LevelData ***"), (uint32)Task.Type);
					break;
				case ETBSMultiThreadingTask::Bonus:
					UE_LOG(LogClass, Log, TEXT("*** Thread: Finished loading BonusData ***"), (uint32)Task.Type);
					break;
			}
		}
	}
	if (Rdycounter > 0 && Rdycounter == MTTasks.Num()) {
		MTTasks[0].Runnable->Shutdown();
		MTTasks.Empty();
		UE_LOG(LogClass, Log, TEXT("*** Thread finished and shut down ***"));
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
		CurrentCustomer->SetActorHiddenInGame(false);
	}
	if (SecondCustomer){
		NextCustomer = SecondCustomer;
		NextCustomer->SetActorHiddenInGame(true);
	}

	if (World && ScreenCaptureCustomer == NULL && ScreenCaptureCustomerClass != NULL) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoCollisionFail = true;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		ScreenCaptureCustomer = World->SpawnActor<ATBSCustomer>(ScreenCaptureCustomerClass, FVector(1200, -200, -1000), FRotator::ZeroRotator, SpawnParams);
	}

	// Spawn Tool
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator ();
		SpawnParams.bNoCollisionFail = true;

		FVector SpawnLocation = {0, 0, 0};
		FRotator SpawnRotation = {180, 0, 0};
		Tool = World->SpawnActor<ATBSRazor>(ToolClass, SpawnLocation, SpawnRotation, SpawnParams);
		Tool->SetActorHiddenInGame(false);
	}
	if (Controller) {
		((ATBSPlayerController*)Controller)->PlayerCharacter = this;
		BeardList = ((ATBSPlayerController*)Controller)->GetBeardNameLevelData();
		CheckBeardUnlocks();
		NewUnlocks.Empty();
	}


}

void ATBSCharacter::StartGame() {
	if (EquipedItems.Num() == 0) {
		// Equip Basic Items
		FTBSItem ItemClipper;
		ItemClipper.EquipmentID = 0;
		ItemClipper.Type = ETBSEquipmentType::Clipper;

		FTBSItem ItemRazor_L;
		ItemRazor_L.EquipmentID = 3;
		ItemRazor_L.Type = ETBSEquipmentType::RazorLarge;

		FTBSItem ItemRazor_S;
		ItemRazor_S.EquipmentID = 4;
		ItemRazor_S.Type = ETBSEquipmentType::RazorSmall;

		FTBSItem ItemTowel;
		ItemTowel.EquipmentID = 9;
		ItemTowel.Type = ETBSEquipmentType::Towel;

		EquipedItems.Add(ItemClipper);
		EquipedItems.Add(ItemRazor_L);
		EquipedItems.Add(ItemRazor_S);
		EquipedItems.Add(ItemTowel);

		// Check If Items Are Already In Saved Files
		ATBSGameState* bla = (ATBSGameState*)GetWorld()->GameState;
		if (bla) {
			UTBSSaveGame* SaveGame = bla->CurrentSaveGame;
			if (SaveGame) {
				if (SaveGame->ObtainedEquipment.Num() == 0) {
					((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Add(ItemClipper.EquipmentID);
					((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Add(ItemRazor_L.EquipmentID);
					((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Add(ItemRazor_S.EquipmentID);
					((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Add(ItemTowel.EquipmentID);
				}
			}
		}
	}


	GameIsRunning = true;
	GameStarted = true;
	CurrentCustomer->CreateNewCustomer(CurrentLevel);
	CurrentBeardRow = ((ATBSPlayerController*)GetController())->FindDataRowToName(CurrentCustomer->DesiredBeard);

	ScreenCaptureCustomer->CreateNewCustomer(CurrentLevel);
	ScreenCaptureCustomer->IsCurrentCustomer = true;
	ScreenCaptureCustomer->SetActorHiddenInGame(false);
	ScreenCaptureCustomer->Beard->SetActorHiddenInGame(false);

	// Load Level Up Data
	const FLevelUpData *CurrentData;
	const FString String = "";
	if (LevelData) {
		FName NameCurrentLevel = FName(*(FString::FromInt(CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData>(NameCurrentLevel, String);
		if (CurrentData) {
			CurrentExperienceToLvl = CurrentData->XPtoLvl;
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle, TimeLimit, false, -1.f);

	// Load Data with another thread
	BeardData_MT.Empty();
	EquipmentData_MT.Empty();
	LevelData_MT.Empty();
	TimeBonusData_MT.Empty();
	
	if (CurrentCustomer) {
		FName DesiredCustomerBeard = CurrentCustomer->DesiredBeard;
		UE_LOG(LogClass, Warning, TEXT("*** %s ***"), *DesiredCustomerBeard.ToString());
		UDataTable* BeardDataTable = ((ATBSPlayerController*)GetController())->FindDataTableToName(DesiredCustomerBeard);
		if (BeardDataTable) {
			FMTTask Task;
			Task.Runnable = FTBSDataLoadWorker::JoyInitBeardComp(BeardData_MT, BeardDataTable);
			Task.Type = ETBSMultiThreadingTask::BeardComparison;
			Task.TaskStarted = true;
			GetWorldTimerManager().SetTimer(Task.Handle, 0.1f, true, -1.f);
			MTTasks.Add(Task);
		}
	}

	if (EquipmentData) {
		FMTTask Task;
		Task.Runnable = FTBSDataLoadWorker::JoyInitEquipment(EquipmentData_MT, EquipmentData);
		Task.Type = ETBSMultiThreadingTask::Equipment;
		Task.TaskStarted = true;
		GetWorldTimerManager().SetTimer(Task.Handle, 0.1f, true, -1.f);
		MTTasks.Add(Task);
	}

	if (LevelData) {
		FMTTask Task;
		Task.Runnable = FTBSDataLoadWorker::JoyInitLevel(LevelData_MT, LevelData);
		Task.Type = ETBSMultiThreadingTask::Level;
		Task.TaskStarted = true;
		GetWorldTimerManager().SetTimer(Task.Handle, 0.1f, true, -1.f);
		MTTasks.Add(Task);
	}

	if (BonusCashData) {
		FMTTask Task;
		Task.Runnable = FTBSDataLoadWorker::JoyInitBonus(TimeBonusData_MT, BonusCashData);
		Task.Type = ETBSMultiThreadingTask::Bonus;
		Task.TaskStarted = true;
		GetWorldTimerManager().SetTimer(Task.Handle, 0.1f, true, -1.f);
		MTTasks.Add(Task);
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

void ATBSCharacter::FinishCurrentCustomer() {
	ResultOpened();
	BeardResult = CalculateResult();
	int32 EXP = (int32)BeardResult;
	LastBeardResult = BeardResult;

	IncreaseEXP(EXP);
	IncreaseCash(BeardResult);

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

	ChangedCustomer();
	Tool->ResetHairs();

	// Setup for next Customer
	SessionID++;
	BeardResult = 0;
	CashEarned = 0;
	CashPenalty = 0;
	LeveledUp = false;		
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
				BeardData_MT.Empty();
				FMTTask Task;
				Task.Runnable = FTBSDataLoadWorker::JoyInitBeardComp(BeardData_MT, BeardDataTable);
				Task.Type = ETBSMultiThreadingTask::BeardComparison;
				Task.TaskStarted = true;
				GetWorldTimerManager().SetTimer(Task.Handle, 0.1f, true, -1.f);
				MTTasks.Add(Task);
			}
		}

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
		ElapsedTime = TimeElapsed;
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
	const FString String = "";
	if (LevelData) {
		FName NameCurrentLevel = FName (*(FString::FromInt (CurrentLevel)));
		CurrentData = LevelData->FindRow<FLevelUpData> (NameCurrentLevel, String);
		if (CurrentData) {
			CurrentExperience += Value;
			if (CurrentExperience >= CurrentData->XPtoLvl) {
				CurrentLevel++;
				CurrentExperience -= CurrentData->XPtoLvl;
				LeveledUp = true;
				CheckBeardUnlocks();

				for (FLevelUpData* Data : LevelData_MT) {
					if (Data->Level == CurrentLevel) {
						CurrentExperienceToLvl = Data->XPtoLvl;
						break;
					}
				}
			}
		}
	}
}

void ATBSCharacter::CheckBeardUnlocks() {
	for (FBeardNameLevelData Beard : BeardList) {
		if (Beard.BeardLevel <= CurrentLevel && !UnlockedBeards.Contains(Beard.UniqueID)) {
			UnlockedBeards.Add(Beard.UniqueID);
			NewUnlocks.Add(Beard.UniqueID);
		}
	}
}


void ATBSCharacter::ApplyCashPenalty(int32 Value) {
	if (CurrentCustomer) {
		CurrentCustomer->HurtCustomer();
	}
	CashPenalty += Value;
	UE_LOG(LogClass, Log, TEXT("*** %d $ penalty for hurting the customer!***"), Value);
}

int32 ATBSCharacter::GetCashEarned() {
	return CashEarned;
}

int32 ATBSCharacter::GetCashPenalty() {
	return CashPenalty;
}

void ATBSCharacter::IncreaseCash(float ComparisionResult) {
	CashEarned = 0;
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

	CalculateBonusCash();

	if (CashEarned - CashPenalty < 0) {
		CashPenalty = CashEarned;
		UE_LOG(LogClass, Log, TEXT("*** Player earned 0$! ***"), );
	}
	else {
		CurrentCash += CashEarned - CashPenalty;
		UE_LOG(LogClass, Log, TEXT("*** Player earned %d$! ***"), CashEarned - CashPenalty);
	}
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
			CashEarned += TimeBonusData_MT[i]->BonusCash;
			UE_LOG(LogClass, Log, TEXT("*** Player earned %d $ as a bonus! ***"), TimeBonusData_MT[i]->BonusCash);
			return;
		}
	}
}

// Returns the comparison Result from the shaved beard of the customer and the CSV data
float ATBSCharacter::CalculateResult () {
	if (Tool  && Tool->InstancedSMComponent) {
		TArray<UActorComponent*> Components;
		int32 Total = Tool->InstancedSMComponent->GetInstanceCount();
		int32 TotalTarget = 0;
		int32 NumShaved = 0;
		int32 NumTargetShaved = 0;
		int32 NumTrimmed = 0;
		int32 NumTargetTrimmed = 0;
		FTransform Transform;

		int32 Correct = 0;

		
		int32 TrimmedIndex = 0;
		for (int32 i = 0; i < Tool->InstancedSMComponent->GetInstanceCount(); i++) {
			int32 State = 2;
			if (Tool->InstancedSMComponent->GetInstanceTransform(i, Transform)) {
				if (Transform.GetLocation().Z >= 1000) { // Shaved
					State = 0;
				}
				else if (Transform.GetLocation().Z <= -1000) { // Trimmed
					if (Tool->InstancedSMComponent->GetInstanceTransform(TrimmedIndex, Transform)) {
						if (Transform.GetLocation().Z >= 1000) { // Shaved
							State = 0;
						}
						else if (Transform.GetLocation().Z <= -1000) { // Trimmed
							State = 1;
						}
					}
					TrimmedIndex++;
				}
			}
			if (BeardData_MT[i]) {
				if (State == BeardData_MT[i]->HairState) {
					Correct++;
				}
				TotalTarget++;
			}

		}

		if (TotalTarget != Total) {
			UE_LOG(LogClass, Warning, TEXT("*** Possible missmatch of Haircount in Beard(%d) and BeardCSVData(%d)! ***"), Total, TotalTarget);
		}

		float Result = ((float)Correct / (float)Total) * 100;

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
		sessionState.TimesHurt = CurrentCustomer->TimesHurt;
		sessionState.SatisfiedCustomer = CurrentCustomer->SatisfiedCustomer;
		sessionState.BeardResult = BeardResult;
		sessionState.TimeRequired = GetTimeElapsed();

		CurrentCustomer->TimesHurt = 0;
		CurrentCustomer->SatisfiedCustomer = false;

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
	const FString Context = FString("");
	if (EquipmentData) {
		FString String = FString::FromInt(ID);
		FName Row = FName(*String);
		CurrentData = EquipmentData->FindRow<FTBSEquipmentData>(Row, Context, false);
		if (CurrentData && CurrentData->Cost <= CurrentCash) {
			CurrentCash -= CurrentData->Cost;
			((ATBSGameState*)(GetWorld()->GameState))->CurrentSaveGame->ObtainedEquipment.Add(ID);
			EquipItem(ID);
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
			Data.Type = EquipmentData_MT[i]->Type;
			Data.EquipmentID = EquipmentData_MT[i]->EquipmentID;
			EquipmentList.Add(Data.EquipmentID, Data);
		}
	}
	return EquipmentList;
}

TArray<FTBSItem> ATBSCharacter::GetEquipedItems() {
	return EquipedItems;
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
		ETBSEquipmentType Type = GetEquipmentByID(ID).Type;
		bool ItemFound = false;
		for (FTBSItem &Item : EquipedItems) {
			if (Item.Type == Type) {
				Item.EquipmentID = ID;
				break;
			}
		}
		EquipedItem(ID);
		return true;
	}
}

#pragma endregion Equipment

#pragma region Beard
TArray<FBeardNameLevelData> ATBSCharacter::GetBeards() {
	TArray<FBeardNameLevelData> Beards;
	if (BeardList.Num() > 0) {
		return BeardList; 
	}
	else {
		if (Controller){
			Beards = BeardList = ((ATBSPlayerController*)GetController())->GetBeardNameLevelData();
		}
		return Beards;
	}
}

FBeardNameLevelData ATBSCharacter::GetBeardByID(int32 ID) {
	FBeardNameLevelData TMPBeard;
	for (FBeardNameLevelData Beard : BeardList) {
		if (Beard.UniqueID == ID) {
			TMPBeard = Beard;
			break;
		}
	}
	return TMPBeard;
}

bool ATBSCharacter::IsBeardUnlocked(int32 ID) {
	return UnlockedBeards.Contains(ID);
}

TArray<int32> ATBSCharacter::GetNewBeardUnlocks() {
	TArray<int32> TMPNewUnlocks = NewUnlocks;
	NewUnlocks.Empty();
	return TMPNewUnlocks;
}


#pragma endregion


float ATBSCharacter::GetCustomerTime() {
	if (CurrentCustomer) {
		return CurrentCustomer->CustomerTimeLimit;
	}
	return -99;
}
float ATBSCharacter::GetCustomerSatisfaction() {
	if (CurrentCustomer) {
		return  CurrentCustomer->CustomerSatisfaction;
	}
	return -99;
}