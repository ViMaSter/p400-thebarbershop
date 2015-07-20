// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSCharacter.h"
#include "TBSCustomer.h"
#include "TBSRazor.h"
#include "TBSPlayerController.h"
#include "TBSGameState.h"
#include "TBSDataLoadWorker.h"

ATBSPlayerController::ATBSPlayerController (const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer) {
	bShowMouseCursor = false;
	ShaveActive = false;
	RotationActive = false;

	PointingAtCustomer = false;

	ToolRotationTarget = FRotator (0, 0, 180);

	RazorRotationLerpIntensity = 1.0f;
	RazorPositionLerpIntensity = 1.0f;
	RazorLoweringLerpIntensity = 1.0f;
	ShavingThreshold = 0.1f;

	PrimaryActorTick.bCanEverTick = true;
}


void ATBSPlayerController::BeginPlay () {
	Super::BeginPlay ();
	//PlayerCharacter = (ATBSCharacter*) GetPawn ();
}

void ATBSPlayerController::PlayerTick (float DeltaTime) {
	Super::PlayerTick (DeltaTime);

	if (!GetWorld()->GetGameState<ATBSGameState>() || !GetWorld()->GetGameState<ATBSGameState>()->GetIsIngame()) {
		return;
	}

	if (PlayerCharacter && PlayerCharacter->Tool && !GetIsPaused()) {
		UpdateMechanicChecks();

		UpdateRazor (DeltaTime);
		UpdateCamera (DeltaTime);

		ApplyRazor (DeltaTime);
		ApplyCamera (DeltaTime);

		bShowMouseCursor = !PointingAtCustomer;
	}

	if (LoadingStarted_MT && GetWorldTimerManager().TimerExists(LoadingTimeHandle_MT)) {
		if (FTBSDataLoadWorker::IsThreadFinished()) {
			FTBSDataLoadWorker::Shutdown();
			LoadingStarted_MT = false;
			LoadBeardToCustomer(BeardData_MT);
			GetWorldTimerManager().ClearTimer(LoadingTimeHandle_MT);
		}
	}
}

void ATBSPlayerController::SetupInputComponent () {
	// set up gameplay key bindings
	Super::SetupInputComponent ();

	InputComponent->BindAxis("RotateTop", this, &ATBSPlayerController::RotateTop);
	InputComponent->BindAxis("RotateRight", this, &ATBSPlayerController::RotateRight);
	InputComponent->BindAxis("RotateRazorTop", this, &ATBSPlayerController::RotateToolTop);

	InputComponent->BindAction("SwitchToNextTool", IE_Pressed, this, &ATBSPlayerController::SwitchToNextTool);
	InputComponent->BindAction("SwitchToPrevTool", IE_Pressed, this, &ATBSPlayerController::SwitchToPrevTool);
	InputComponent->BindAction("SwitchToShortRazor", IE_Pressed, this, &ATBSPlayerController::SwitchToShortRazor);
	InputComponent->BindAction("SwitchToNormalRazor", IE_Pressed, this, &ATBSPlayerController::SwitchToNormalRazor);
	InputComponent->BindAction("SwitchToClipper", IE_Pressed, this, &ATBSPlayerController::SwitchToClipper);

	InputComponent->BindAction("Shave", IE_Pressed, this, &ATBSPlayerController::OnSetShavedPressed);
	InputComponent->BindAction("Shave", IE_Released, this, &ATBSPlayerController::OnSetShavedReleased);
	InputComponent->BindAction("Rotate", IE_Pressed, this, &ATBSPlayerController::OnSetRotationPressed);
	InputComponent->BindAction("Rotate", IE_Released, this, &ATBSPlayerController::OnSetRotationReleased);

	// Cheat Codes Pitch Hack
	InputComponent->BindAction("FinishCustomer", IE_Pressed, this, &ATBSPlayerController::FinishCurrentCustomer);
	InputComponent->BindAction("SpawnNextCustomer", IE_Pressed, this, &ATBSPlayerController::SpawnNextCustomer);
	
	InputComponent->BindAction("MoveChair", IE_Pressed, this, &ATBSPlayerController::LiftPositionPressed);
	InputComponent->BindAction("MoveChair", IE_Released, this, &ATBSPlayerController::LiftPositionReleased);
}

#pragma region Camera Control
void ATBSPlayerController::ResetCamera() {
	float yaw = 180 - ((ATBSCharacter*)GetPawn())->CameraBoom->RelativeRotation.Yaw;
	float pitch = -(((ATBSCharacter*)GetPawn())->CameraBoom->RelativeRotation.Pitch);
	CameraRotationTarget.Add(pitch, yaw, 0);
	PlayerCharacter->CameraBoom->SetRelativeRotation(CameraRotationTarget);
	PlayerCharacter->CameraBoom->AddRelativeRotation(FRotator(0, 180, 0));
}

void ATBSPlayerController::RotateCamera (float Pitch, float Yaw) {
	if (PlayerCharacter && !bCinematicMode) {
		CameraRotationTarget.Add (Pitch, Yaw, 0);

		CameraRotationTarget.Pitch = FMath::ClampAngle (CameraRotationTarget.Pitch, PlayerCharacter->VerticalUpperCameraRotationBorder, PlayerCharacter->VerticalLowerCameraRotationBorder);
		CameraRotationTarget.Yaw = FMath::ClampAngle (CameraRotationTarget.Yaw, -PlayerCharacter->HorizontalCameraRotationBorder, PlayerCharacter->HorizontalCameraRotationBorder);
	}
}

void ATBSPlayerController::RotateTop (float Value) {
	if (!GetWorld()->GetGameState<ATBSGameState>() || !GetWorld()->GetGameState<ATBSGameState>()->GetIsIngame()) {
		return;
	}

	RotateCamera (-Value, 0);
}

void ATBSPlayerController::RotateRight (float Value) {
	if (!GetWorld()->GetGameState<ATBSGameState>() || !GetWorld()->GetGameState<ATBSGameState>()->GetIsIngame()) {
		return;
	}

	RotateCamera (0, -Value);
}
#pragma endregion

#pragma region Tool Control
void ATBSPlayerController::RotateToolTop (float Value) {
	if (RotationActive && Value != 0.f) {
		Value *= -10;
		ToolRotationTarget += FRotator (0, 0, Value);
	}
}

void ATBSPlayerController::OnSetShavedPressed() {
	if (RotationActive)
		return;

	ShaveActive = true;
}

void ATBSPlayerController::OnSetShavedReleased() {
	if (RotationActive)
		return;

	ShaveActive = false;
	SaveStep();
}

void ATBSPlayerController::OnSetRotationPressed() {
	if (ShaveActive)
		return;

	RotationActive = true;

	GetMousePosition (StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::OnSetRotationReleased() {
	if (ShaveActive)
		return;

	ToolRotationTarget = PlayerCharacter->Tool->GetActorRotation ();

	RotationActive = false;
	ULocalPlayer* LocalPlayer = CastChecked<ULocalPlayer> (Player);
	FViewport* ViewPort = LocalPlayer->ViewportClient->Viewport;
	ViewPort->SetMouse (StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::SwitchToNextTool () {
	if (!GetWorld()->GetGameState<ATBSGameState>() || !GetWorld()->GetGameState<ATBSGameState>()->GetIsIngame()) {
		return;
	}

	if (GetIsPaused()) {
		return;
	}

	// Save the fact, that the user "discovered" the other tools
	ATBSGameState* GameState = GetWorld()->GetGameState<ATBSGameState>();

	if ( !(GameState->CurrentSaveGame->UsedOtherTools) )
		GameState->CurrentSaveGame->UsedOtherTools = true;

	// Actually switch tools
	PlayerCharacter->SwitchTool(true);
}

void ATBSPlayerController::SwitchToPrevTool () {
	if (!GetWorld()->GetGameState<ATBSGameState>() || !GetWorld()->GetGameState<ATBSGameState>()->GetIsIngame()) {
		return;
	}

	if (GetIsPaused()) {
		return;
	}

	// Save the fact, that the user "discovered" the other tools
	ATBSGameState* GameState = GetWorld()->GetGameState<ATBSGameState>();

	if ( !(GameState->CurrentSaveGame->UsedOtherTools) )
		GameState->CurrentSaveGame->UsedOtherTools = true;

	PlayerCharacter->SwitchTool (false);
}

void ATBSPlayerController::SwitchToShortRazor(){
	if (PlayerCharacter && PlayerCharacter->Tool && PlayerCharacter->GameIsRunning) {
		PlayerCharacter->Tool->SwitchRazorTypeTo(ETBSRazor::TBSRazorSmall);
	}
}

void ATBSPlayerController::SwitchToNormalRazor(){
	if (PlayerCharacter && PlayerCharacter->Tool && PlayerCharacter->GameIsRunning) {
		PlayerCharacter->Tool->SwitchRazorTypeTo(ETBSRazor::TBSRazorBig);
	}
}

void ATBSPlayerController::SwitchToClipper(){
	if (PlayerCharacter && PlayerCharacter->Tool && PlayerCharacter->GameIsRunning) {
		PlayerCharacter->Tool->SwitchRazorTypeTo(ETBSRazor::TBSClipper);
	}
}

#pragma endregion

#pragma region Tick
void ATBSPlayerController::UpdateRazor (float DeltaTime) {
	if (!GetWorld()->GetGameState<ATBSGameState>() || !GetWorld()->GetGameState<ATBSGameState>()->GetIsIngame()) {
		return;
	}

	if (PlayerCharacter) {
		if (!RotationActive) {
			// Get hit from mouse cursor ray
			FHitResult Hitresult;
			GetHitResultUnderCursor (ECC_Camera, true, Hitresult);

			if (Hitresult.GetActor()) {
				if(Hitresult.GetActor()->GetClass()->IsChildOf(ATBSCustomer::StaticClass())) {
					// Save hitresult info
					LastValidMouseCursorImpactPoint = Hitresult.ImpactPoint;
					LastValidMouseCursorImpactNormal = Hitresult.ImpactNormal;

					// SKIN OFFSET
					// Lower/raise the tool based on ::ShaveActive
					if (ShaveActive) {
						ToolHeightOffsetTarget = FVector::ZeroVector;
					}
					else {
						ToolHeightOffsetTarget = Hitresult.ImpactNormal*PlayerCharacter->Tool->ToolInactiveHight;
					}
					ToolHeightOffsetCurrent = FMath::Lerp (ToolHeightOffsetCurrent, ToolHeightOffsetTarget, (1.0f / DeltaTime / 60.0f) * RazorLoweringLerpIntensity);

					// Whether or not the tool is active, is now dependent on the razors distance from the skin
					PlayerCharacter->Tool->IsActive = ToolHeightOffsetCurrent.Size() < ShavingThreshold && IsMovingShave;

					// POSITION
					ToolLocationTarget = Hitresult.ImpactPoint + ToolHeightOffsetCurrent;

					// ROTATION
					ToolRotationTarget.Pitch = Hitresult.ImpactNormal.Rotation ().Pitch;
					ToolRotationTarget.Yaw = Hitresult.ImpactNormal.Rotation ().Yaw - 180;

					PointingAtCustomer = true;
				}
				else if (!Hitresult.GetActor()->GetClass()->IsChildOf(ATBSRazor::StaticClass())) {
					// Fall back to our default position
					ToolLocationTarget = PlayerCharacter->ToolResetPosition->GetComponentLocation ();
					ToolRotationTarget = PlayerCharacter->ToolResetPosition->GetComponentRotation ();

					PointingAtCustomer = false;
				}
			}
		}
	}
}

void ATBSPlayerController::ApplyRazor (float DeltaTime) {
	// Fetch current values
	ToolRotationCurrent = PlayerCharacter->Tool->GetActorRotation();
	ToolLocationCurrent = PlayerCharacter->Tool->GetActorLocation();

	// Could handle special PointingAtCustomer-case here
	PlayerCharacter->Tool->SetActorLocation(
		FMath::Lerp(ToolLocationCurrent, ToolLocationTarget, (1.0f / DeltaTime / 60.0f) * RazorPositionLerpIntensity), IsSweapMovement
		);

	PlayerCharacter->Tool->SetActorRotation(
		FMath::Lerp(ToolRotationCurrent, ToolRotationTarget, (1.0f / DeltaTime / 60.0f) * RazorRotationLerpIntensity)
		);
}

void ATBSPlayerController::UpdateCamera (float DeltaTime) {
	// Could modify CameraRotationTarget and CameraLocationTarget before it's ultimately being used here
}
void ATBSPlayerController::ApplyCamera (float DeltaTime) {
	PlayerCharacter->CameraBoom->SetRelativeRotation (FMath::Lerp (PlayerCharacter->CameraBoom->RelativeRotation - FRotator (0, 180, 0), CameraRotationTarget, PlayerCharacter->CameraRotationLerpIntensity));
	PlayerCharacter->CameraBoom->AddRelativeRotation (FRotator (0, 180, 0));
}

void ATBSPlayerController::UpdateMechanicChecks() {
	FVector CurrentToolDirection = PlayerCharacter->Tool->GetTransform().InverseTransformVector(ToolLocationTarget - ToolLocationCurrent);
	
	float value = FMath::Atan2(CurrentToolDirection.Y, CurrentToolDirection.Z);

	/// IsLegalShaveAngle
	IsLegalShaveAngle = FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(CurrentToolDirection.Y, CurrentToolDirection.Z))) <= (LegalShaveAngle / 2);

	// IsMovingShave
	IsMovingShave = FMath::Abs(CurrentToolDirection.Y + CurrentToolDirection.Z) >= MinimumShaveSpeed;

}
#pragma endregion

#pragma region Pitch Hacks
void ATBSPlayerController::SpawnNextCustomer () {
	if (GetWorld()->GetGameState<ATBSGameState>()) {
		GetWorld()->GetGameState<ATBSGameState>()->SetIsPaused(false);
	}

	SpawnedNextCustomer();
	if (PlayerCharacter) {
		PlayerCharacter->TransitionToNewCustomer();
	}
}

void ATBSPlayerController::FinishCurrentCustomer() {
	if (GetIsEditorMode()) {
		return;
	}

	if (GetIsPaused()) {
		UE_LOG(LogClass, Log, TEXT("*** Game is paused! ***"));
		return;
	}

	if (PlayerCharacter) {
		PlayerCharacter->FinishCurrentCustomer();
	}

	if (GetWorld()->GetGameState<ATBSGameState>()) {
		GetWorld()->GetGameState<ATBSGameState>()->SetIsPaused(true);
		UpdateCurrentSaveGame();
	}
}
#pragma endregion

#pragma region Lift
void ATBSPlayerController::LiftPositionPressed() {
	if (GetIsPaused()) {
		UE_LOG(LogClass, Log, TEXT("*** Game is paused! ***"));
		return;
	}
	((ATBSCharacter*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->CurrentCustomer->LiftPositionPressed();
}

void ATBSPlayerController::LiftPositionReleased() {
	if (GetIsPaused()) {
		UE_LOG(LogClass, Log, TEXT("*** Game is paused! ***"));
		return;
	}
	((ATBSCharacter*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->CurrentCustomer->LiftPositionReleased();
}
#pragma endregion

#pragma region Beard Data Management
// Helper function cause C++ definition of Modulo operator is different (-1 % 10 = -1 in c++. in other languages its -1 % 10 = 9)
// Works for negative and positive Value and positive Divider!
static int32 Modulo(int32 Value, int32 Divider) {
	int32 tmp = Value % Divider;
	if (tmp < 0) {
		tmp += Divider;
	}
	return tmp;
}

// Saves the last Step for Undo/Redo purpose
bool ATBSPlayerController::SaveStep() {
	if (PlayerCharacter && ChangedBeard && GetIsEditorMode()) {
		ChangedBeard = false;
		bool success = false;

		StepIndex++;
		StepIndex = Modulo(StepIndex,MAXREDOSTEPS);

		UDataTable* StepData;
		StepData = PlayerCharacter->RedoUndoData[StepIndex];
		success = SetCurrentBeardDataToCSV(StepData);

		if (TotalSteps < MAXREDOSTEPS) {
			TotalSteps++;
		}
		TotalUndoedSteps = 0;							// If saved Step reset UndoedSteps. Never returning back after Undoing and Saving again
		return success;
	}
	return false;
}

void ATBSPlayerController::SetChangedBeard() {
	ChangedBeard = true;
}

// Call from HUD
bool ATBSPlayerController::RedoBeardChanges() {
	if (PlayerCharacter && GetIsEditorMode()) {
		if (TotalUndoedSteps > 0) {					// Make sure u can redo as much as u had undoed
			bool success = false;
			StepIndex++;
			StepIndex = Modulo(StepIndex, MAXREDOSTEPS);

			UDataTable* RedoStepData;
			RedoStepData = PlayerCharacter->RedoUndoData[StepIndex];
			success = LoadBeardDataToCurrentCustomer(RedoStepData);

			TotalSteps++;
			TotalUndoedSteps--;
			return success;
		}
	}
	return false;
}

// Call from HUD
bool ATBSPlayerController::UndoBeardChanges() {
	if (PlayerCharacter && GetIsEditorMode()) {
		if (TotalSteps > 1) {			// Make sure maximal MAXREDOSTEPS-1 are possible. We cant go further than the 1st real step
			bool success = false;
			StepIndex--;
			StepIndex = Modulo(StepIndex, MAXREDOSTEPS);

			UDataTable* UndoStepData;
			UndoStepData = PlayerCharacter->RedoUndoData[StepIndex];
			success = LoadBeardDataToCurrentCustomer(UndoStepData);

			TotalSteps--;
			TotalUndoedSteps++;
			return success;
		}
	}
	return false;
}


bool ATBSPlayerController::ClearBeardData() {
	if (!GetIsEditorMode()) {
		UE_LOG(LogClass, Log, TEXT("*** No editor mode active ***"));
		return false;
	}
	TArray<FBeardNameLevelData> Beards = GetBeardNameLevelData();
	if (PlayerCharacter){
		for (int32 i = 0; i < Beards.Num(); i++) {
			ClearBeardID(Beards[i].BeardName);
		}
	}
	if (Beards.Num() == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool ATBSPlayerController::ClearBeardID(FName BeardName) {
	if (!GetIsEditorMode()) {
		UE_LOG(LogClass, Log, TEXT("*** No editor mode active ***"));
		return false;
	}
	if (PlayerCharacter) {
		UDataTable* DataTable;
		DataTable = FindDataTableToName (BeardName);
		if (DataTable) {
			DataTable->EmptyTable ();
		}
		return RemoveBeardFromCollection (BeardName);
	}
	return false;
}

bool ATBSPlayerController::SaveBeardID(FName BeardName, int32 BeardLevel, int32 UniqueId) {
	if (!GetIsEditorMode()) {
		UE_LOG(LogClass, Log, TEXT("*** No editor mode active ***"));
		return false;
	}
	if (PlayerCharacter) {
		bool success = true;
		UDataTable* DataTable;
		success = SetBeardToCollectionData(BeardName, BeardLevel , UniqueId);
		DataTable = FindDataTableToName (BeardName);
		if (DataTable) {
			return success && SetCurrentBeardDataToCSV(DataTable);
		}
	}
	return false;
}

bool ATBSPlayerController::LoadBeardID(FName BeardName) {
	if (!GetIsEditorMode()) {
		UE_LOG(LogClass, Log, TEXT("*** No editor mode active ***"));
		return false;
	}
	if (PlayerCharacter) {
		UDataTable* DataTable;
		DataTable = FindDataTableToName (BeardName);
		if (DataTable) {
			return LoadBeardDataToCurrentCustomer (DataTable);
		}
	}
	return false;
}

void ATBSPlayerController::LoadBeardID_MT(FName BeardName) {
	if (!GetIsEditorMode()) {
		UE_LOG(LogClass, Log, TEXT("*** No editor mode active ***"));
		return;
	}
	if (PlayerCharacter) {
		UDataTable* DataTable;
		DataTable = FindDataTableToName(BeardName);
		if (DataTable) {
			FTBSDataLoadWorker::JoyInitBeardComp(BeardData_MT, DataTable);
			GetWorldTimerManager().SetTimer(LoadingTimeHandle_MT, 0.1f, true, -1.f);
			LoadingStarted_MT = true;
		}
	}
}

bool ATBSPlayerController::SetCurrentBeardDataToCSV(UDataTable* DataTable) {
	if (PlayerCharacter == NULL) {
		return false;
	}
	TArray<UActorComponent*> Components;
	Components = PlayerCharacter->CurrentCustomer->Beard->GetComponentsByClass (UStaticMeshComponent::StaticClass ());
	FBeardComparisonData* CurrentData;
	for (int32 i = 0; i < Components.Num (); i++) {
		int32 ComponentStatus;
		const FString Context;
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
		CurrentData = DataTable->FindRow<FBeardComparisonData> (Row, Context, false);

		if (CurrentData) {
			CurrentData->HairState = ComponentStatus;
		}
		else {
			UScriptStruct* LoadUsingStruct = FBeardComparisonData::StaticStruct ();
			uint8* RowData = (uint8*) FMemory::Malloc (LoadUsingStruct->PropertiesSize);
			DataTable->RowMap.Add (Row, RowData);
			CurrentData = DataTable->FindRow<FBeardComparisonData> (Row, Context, false);
			if (CurrentData) {
				CurrentData->HairState = ComponentStatus;
			}
			else{
				return false;
			}
		}
	}
	return true;
}

bool ATBSPlayerController::LoadBeardDataToCurrentCustomer (UDataTable* DataTable) {
	if (PlayerCharacter == NULL) {
		return false;
	}
	TArray<UActorComponent*> Components;
	Components = PlayerCharacter->CurrentCustomer->Beard->GetComponentsByClass (UStaticMeshComponent::StaticClass ());
	FBeardComparisonData* CurrentData;
	const FString Context;
	bool success = true;
	for (int32 i = 0; i < Components.Num (); i++) {
		FString String = FString::FromInt (i);
		FName Row = FName (*String);
		CurrentData = DataTable->FindRow<FBeardComparisonData> (Row, Context, false);
		if (CurrentData) {
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*) Components[i];
			switch (CurrentData->HairState) {
				case(0) :
					Mesh->SetVisibility (false);
					Mesh->SetCollisionResponseToAllChannels (ECR_Ignore);
					PlayerCharacter->Tool->Trimmed (1, Components[i]);
					break;
				case(1) :
					Mesh->SetVisibility (true);
					Mesh->SetCollisionResponseToAllChannels (ECR_Ignore);
					PlayerCharacter->Tool->Trimmed (0.8, Components[i]);

					break;
				case(2) :
					Mesh->SetVisibility (true);
					Mesh->SetCollisionResponseToAllChannels (ECR_Overlap);
					PlayerCharacter->Tool->Trimmed (0, Components[i]);
					break;
			}
		}
		else {
			UE_LOG (LogClass, Warning, TEXT ("*** Could not load Beard Data Row! Possible missmatch of Meshcount ***"));
			success = false;
		}
	}
	return success;
}

void ATBSPlayerController::LoadBeardToCustomer(TArray<FBeardComparisonData*> Data) {
	if (PlayerCharacter == NULL) {
		return;
	}
	TArray<UActorComponent*> Components;
	Components = PlayerCharacter->CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (int32 i = 0; i < Components.Num(); i++) {
		if (Data[i]) {
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
			switch (Data[i]->HairState) {
			case(0) :
				Mesh->SetVisibility(false);
				Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
				PlayerCharacter->Tool->Trimmed(1, Components[i]);
				break;
			case(1) :
				Mesh->SetVisibility(true);
				Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
				PlayerCharacter->Tool->Trimmed(0.8, Components[i]);

				break;
			case(2) :
				Mesh->SetVisibility(true);
				Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
				PlayerCharacter->Tool->Trimmed(0, Components[i]);
				break;
			}
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** Possible missmatch of meshcount! Array of FBeardComparisonData from thread has not enough rows! ***"));
		}
	}
}

FBeardCollectionData ATBSPlayerController::FindDataRowToName(FName BeardName) {
	if (PlayerCharacter) {
		FBeardCollectionData CurrentData;
		FName DataTableName;
		if (PlayerCharacter->BeardCollection) {
			const FString Context;
			for (int32 i = 0; i < PlayerCharacter->BeardCollection->GetRowNames().Num(); i++) {
				FName Row = PlayerCharacter->BeardCollection->GetRowNames()[i];
				CurrentData = *(PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData>(Row, Context, false));
				if (CurrentData.BeardName == BeardName) {
					return CurrentData;
				}
			}
			UE_LOG(LogClass, Warning, TEXT("*** Could not find Beard in CollectionData! ***"));
		}
	}
	return FBeardCollectionData();
}

UDataTable* ATBSPlayerController::FindDataTableToName(FName BeardName) {
	if (PlayerCharacter) {
		FBeardCollectionData* CurrentData;
		FName DataTableName;
		if (PlayerCharacter->BeardCollection) {
			const FString Context;
			for (int32 i = 0; i < PlayerCharacter->BeardCollection->GetRowNames().Num(); i++) {
				FName Row = PlayerCharacter->BeardCollection->GetRowNames()[i];
				CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData>(Row, Context, false);
				if (CurrentData) {
					if (CurrentData->BeardName == BeardName) {
						DataTableName = CurrentData->BeardSlotName;
						for (int32 i = 0; i < PlayerCharacter->BeardData.Num(); i++) {
							if (PlayerCharacter->BeardData[i]->GetName() == DataTableName.ToString()) {
								return PlayerCharacter->BeardData[i];
								break;
							}
						}
						break;
					}
				}
				else {
					UE_LOG(LogClass, Warning, TEXT("*** Could not find Beard in CollectionData! ***"));
					break;
				}
			}
		}
	}
	return NULL;
}

bool ATBSPlayerController::RemoveBeardFromCollection (FName BeardName) {
	FBeardCollectionData* CurrentData;
	bool Success = false;
	if (PlayerCharacter->BeardCollection) {
		const FString Context;
		for (int32 i = 0; i < PlayerCharacter->BeardCollection->GetRowNames ().Num (); i++) {
			FName Row = PlayerCharacter->BeardCollection->GetRowNames ()[i];
			CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData> (Row, Context, false);
			if (CurrentData && CurrentData->BeardName == BeardName) {
				PlayerCharacter->BeardCollection->RowMap.Remove (Row);
				Success = true;
				break;
			}
		}
		if (!Success) {
			UE_LOG (LogClass, Warning, TEXT ("*** Could not find Beard in CollectionData! ***"));
		}
	}
	return Success;
}

bool ATBSPlayerController::SetBeardToCollectionData(FName BeardName, int32 BeardLevel, int32 UniqueId) {
	FBeardCollectionData* CurrentData;
	bool Success = false;
	if (PlayerCharacter->BeardCollection) {
		const FString Context;
		TArray<FName> RowNames = PlayerCharacter->BeardCollection->GetRowNames ();
		for (int32 i = 0; i < RowNames.Num (); i++) {
			FName Row = RowNames[i];
			CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData> (Row, Context, false);
			if (CurrentData && CurrentData->BeardName == BeardName) {
				Success = true;
				CurrentData->BeardLevel = BeardLevel;
				CurrentData->UniqueIdentifier = UniqueId;
				break;
			}
		}
		if (!Success) {
			// Find a free ID
			FName NewRowName;
			int32 RowID = 0;
			while (true) {
				bool IsFreeID = true;
				FString RowString = FString::FromInt (RowID);
				NewRowName = FName (*RowString);
				for (int32 i = 0; i < RowNames.Num (); i++) {
					if (RowNames[i] == NewRowName) IsFreeID = false;
				}
				if (IsFreeID) break;
				RowID++;
			}

			// Find Free Slot
			FName NewSlotName;
			int32 SlotID = 0;
			while (true) {
				bool IsFreeSlot = true;
				FString SlotString = "Beard";
				if (SlotID < 10) {
					SlotString.Append ("0");
				}
				SlotString.Append (FString::FromInt (SlotID));
				NewSlotName = FName (*SlotString);
				for (int32 i = 0; i < RowNames.Num (); i++) {
					FName Row = RowNames[i];
					CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData> (Row, Context, false);
					if (CurrentData && CurrentData->BeardSlotName == NewSlotName) IsFreeSlot = false;
				}
				if (IsFreeSlot) break;
				SlotID++;
				// TEMPORARY HARD BREAK DUE TO LIMITED SLOTS
				if (SlotID >= 10) {
					UE_LOG (LogClass, Warning, TEXT ("*** Could not save the beard! ***"));
					UE_LOG (LogClass, Warning, TEXT ("*** All slots are full please delete old beards or add new Slots! ***"));
					return false;
				}
			}

			// Allocate Memory
			UScriptStruct* LoadUsingStruct = FBeardCollectionData::StaticStruct ();
			uint8* RowData = (uint8*) FMemory::Malloc (LoadUsingStruct->PropertiesSize);
			PlayerCharacter->BeardCollection->RowMap.Add (NewRowName, RowData);
			CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData> (NewRowName, Context, false);
			if (CurrentData) {
				CurrentData->BeardName = BeardName;
				CurrentData->BeardSlotName = NewSlotName;
				CurrentData->BeardLevel = BeardLevel;
				CurrentData->UniqueIdentifier = UniqueId;
				CurrentData->ComparisionScreenshot = PlayerCharacter->BeardIcons[RowID];
				return true;
			}
		}
	}
	return Success;
}

// Call from the HUD to get an array of Name, int32 struct of the saved beards
TArray<FBeardNameLevelData> ATBSPlayerController::GetBeardNameLevelData() {
	TArray<FBeardNameLevelData> BeardNameLevelData;
	if (PlayerCharacter == NULL) return BeardNameLevelData;

	FBeardCollectionData* CurrentData;
	if (PlayerCharacter->BeardCollection) {
		const FString Context;
		for (int32 i = 0; i < PlayerCharacter->BeardCollection->GetRowNames().Num(); i++) {
			FName Row = PlayerCharacter->BeardCollection->GetRowNames()[i];
			CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData>(Row, Context, false);

			if (CurrentData) {
				FBeardNameLevelData Data;
				Data.BeardLevel = CurrentData->BeardLevel;
				Data.BeardName = CurrentData->BeardName;
				Data.UniqueID = CurrentData->UniqueIdentifier;
				BeardNameLevelData.Add(Data);
			}
		}
	}
	return BeardNameLevelData;
}

#pragma endregion

#pragma region GameState Wrapper
#pragma region IsPaused
bool ATBSPlayerController::GetIsPaused() {
	ATBSGameState* gameState;
	if (GetWorld()) {
		gameState = GetWorld()->GetGameState<ATBSGameState>();
		if (gameState) {
			return gameState->GetIsPaused();
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** No Game State found! ***"));
		}
	}
	return false;
}

void ATBSPlayerController::SetIsPaused(bool IsPaused) {
	ATBSGameState* gameState;
	if (GetWorld()) {
		gameState = GetWorld()->GetGameState<ATBSGameState>();
		if (gameState) {
			gameState->SetIsPaused(IsPaused);
			if (PlayerCharacter) {
				if (IsPaused) {
					PlayerCharacter->PauseGameTimer();
				}
				else {
					PlayerCharacter->UnpauseGameTimer();
				}
			}
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** No Game State found! ***"));
		}
	}
}
#pragma endregion

#pragma region IsEditorMode
bool ATBSPlayerController::GetIsEditorMode() {
	ATBSGameState* gameState;
	if (GetWorld()) {
		gameState = GetWorld()->GetGameState<ATBSGameState>();
		if (gameState) {
			return gameState->GetIsEditorMode();
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** No Game State found! ***"));
		}
	}
	return false;
}


void ATBSPlayerController::SetIsEditorMode(bool IsEditorMode) {
	ATBSGameState* gameState;
	if (GetWorld()) {
		gameState = GetWorld()->GetGameState<ATBSGameState>();
		if (gameState) {
			gameState->SetIsEditorMode(IsEditorMode);
			if (IsEditorMode) {
				// Make sure to save the first Fullbeard for Undo Step
				SetChangedBeard();
				SaveStep();
			}
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** No Game State found! ***"));
		}
	}
}
#pragma endregion

#pragma region IsIngame
bool ATBSPlayerController::GetIsIngame() {
	ATBSGameState* gameState;
	if (GetWorld()) {
		gameState = GetWorld()->GetGameState<ATBSGameState>();
		if (gameState) {
			return gameState->GetIsIngame();
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** No Game State found! ***"));
		}
	}
	return false;
}


void ATBSPlayerController::SetIsIngame(bool IsIngame) {
	ATBSGameState* gameState;
	if (GetWorld()) {
		gameState = GetWorld()->GetGameState<ATBSGameState>();
		if (gameState) {
			gameState->SetIsIngame(IsIngame);
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("*** No Game State found! ***"));
		}
	}
}
#pragma endregion
#pragma endregion
#pragma region SaveGame Wrapper
void ATBSPlayerController::UpdateCurrentSaveGame() {
	ATBSCharacter* controlledPawn = Cast<ATBSCharacter>(GetPawn());

	GetWorld()->GetGameState<ATBSGameState>()->CurrentSaveGame->ShaveperiencePoints = controlledPawn->CurrentExperience;
	GetWorld()->GetGameState<ATBSGameState>()->CurrentSaveGame->MoneyAvailable = controlledPawn->CurrentCash;
}
#pragma endregion
