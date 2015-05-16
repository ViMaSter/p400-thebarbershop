// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSCustomer.h"
#include "TBSRazor.h"
#include "TBSPlayerController.h"

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
}

void ATBSPlayerController::BeginPlay () {
	Super::BeginPlay ();
	PlayerCharacter = (ATBSCharacter*) GetPawn ();
}

void ATBSPlayerController::PlayerTick (float DeltaTime) {
	Super::PlayerTick (DeltaTime);

	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore) {
		return;
	}

	if (PlayerCharacter) {
		UpdateRazor (DeltaTime);
		UpdateCamera (DeltaTime);

		ApplyRazor (DeltaTime);
		ApplyCamera (DeltaTime);
	}

	bShowMouseCursor = !PointingAtCustomer;
}

void ATBSPlayerController::SetupInputComponent () {
	// set up gameplay key bindings
	Super::SetupInputComponent ();

	InputComponent->BindAxis ("RotateTop", this, &ATBSPlayerController::RotateTop);
	InputComponent->BindAxis ("RotateRight", this, &ATBSPlayerController::RotateRight);
	InputComponent->BindAxis ("RotateRazorTop", this, &ATBSPlayerController::RotateToolTop);

	InputComponent->BindAction ("SwitchToNextTool", IE_Pressed, this, &ATBSPlayerController::SwitchToNextTool);
	InputComponent->BindAction ("SwitchToPrevTool", IE_Pressed, this, &ATBSPlayerController::SwitchToPrevTool);
	InputComponent->BindAction ("Shave", IE_Pressed, this, &ATBSPlayerController::OnSetShavedPressed);
	InputComponent->BindAction ("Shave", IE_Released, this, &ATBSPlayerController::OnSetShavedReleased);
	InputComponent->BindAction ("Rotate", IE_Pressed, this, &ATBSPlayerController::OnSetRotationPressed);
	InputComponent->BindAction ("Rotate", IE_Released, this, &ATBSPlayerController::OnSetRotationReleased);

	// Cheat Codes Pitch Hack
	InputComponent->BindAction ("FinishCustomer", IE_Pressed, this, &ATBSPlayerController::FinishCurrentCustomer);
	InputComponent->BindAction("SpawnNextCustomer", IE_Pressed, this, &ATBSPlayerController::SpawnNextCustomer);
	
	InputComponent->BindAction("MoveChair", IE_Pressed, this, &ATBSPlayerController::LiftPositionPressed);
	InputComponent->BindAction("MoveChair", IE_Released, this, &ATBSPlayerController::LiftPositionReleased);
}

#pragma region Camera Control
void ATBSPlayerController::RotateCamera (float Pitch, float Yaw) {
	if (PlayerCharacter) {
		CameraRotationTarget.Add (Pitch, Yaw, 0);

		CameraRotationTarget.Pitch = FMath::ClampAngle (CameraRotationTarget.Pitch, PlayerCharacter->VerticalUpperCameraRotationBorder, PlayerCharacter->VerticalLowerCameraRotationBorder);
		CameraRotationTarget.Yaw = FMath::ClampAngle (CameraRotationTarget.Yaw, -PlayerCharacter->HorizontalCameraRotationBorder, PlayerCharacter->HorizontalCameraRotationBorder);
	}
}

void ATBSPlayerController::RotateTop (float Value) {
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore) {
		return;
	}

	RotateCamera (-Value, 0);
}

void ATBSPlayerController::RotateRight (float Value) {
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore) {
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

void ATBSPlayerController::OnSetShavedPressed () {
	ShaveActive = true;
}

void ATBSPlayerController::OnSetShavedReleased () {
	ShaveActive = false;
}

void ATBSPlayerController::OnSetRotationPressed () {
	RotationActive = true;

	GetMousePosition (StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::OnSetRotationReleased () {
	ToolRotationTarget = PlayerCharacter->Tool->GetActorRotation ();

	RotationActive = false;
	ULocalPlayer* LocalPlayer = CastChecked<ULocalPlayer> (Player);
	FViewport* ViewPort = LocalPlayer->ViewportClient->Viewport;
	ViewPort->SetMouse (StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::SwitchToNextTool () {
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore) {
		return;
	}

	PlayerCharacter->SwitchTool (true);
}

void ATBSPlayerController::SwitchToPrevTool () {
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore) {
		return;
	}

	PlayerCharacter->SwitchTool (false);
}
#pragma endregion

#pragma region Tick
void ATBSPlayerController::UpdateRazor (float DeltaTime) {
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore) {
		return;
	}

	if (PlayerCharacter) {
		if (!RotationActive) {
			// Get hit from mouse cursor ray
			FHitResult Hitresult;
			GetHitResultUnderCursor (ECC_Camera, true, Hitresult);

			if (Hitresult.GetActor() && (Hitresult.GetActor()->GetClass()->IsChildOf(ATBSRazor::StaticClass()) || Hitresult.GetActor()->GetClass()->IsChildOf(ATBSCustomer::StaticClass()))) {
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
				PlayerCharacter->Tool->IsActive = ToolHeightOffsetCurrent.Size () < ShavingThreshold;

				// POSITION
				ToolLocationTarget = Hitresult.ImpactPoint + ToolHeightOffsetCurrent;

				// ROTATION
				ToolRotationTarget.Pitch = Hitresult.ImpactNormal.Rotation ().Pitch;
				ToolRotationTarget.Yaw = Hitresult.ImpactNormal.Rotation ().Yaw - 180;

				PointingAtCustomer = true;
			}
			else {
				// Fall back to our default position
				ToolLocationTarget = PlayerCharacter->ToolResetPosition->GetComponentLocation ();
				ToolRotationTarget = PlayerCharacter->ToolResetPosition->GetComponentRotation ();

				PointingAtCustomer = false;
			}
		}
	}
}

void ATBSPlayerController::ApplyRazor (float DeltaTime) {
	// Fetch current values
	ToolRotationCurrent = PlayerCharacter->Tool->GetActorRotation ();
	ToolLocationCurrent = PlayerCharacter->Tool->GetActorLocation ();

	// Could handle special PointingAtCustomer-case here
	PlayerCharacter->Tool->SetActorLocation (
		FMath::Lerp (ToolLocationCurrent, ToolLocationTarget, (1.0f / DeltaTime / 60.0f) * RazorPositionLerpIntensity)
		);

	PlayerCharacter->Tool->SetActorRotation (
		FMath::Lerp (ToolRotationCurrent, ToolRotationTarget, (1.0f / DeltaTime / 60.0f) * RazorRotationLerpIntensity)
		);
}

void ATBSPlayerController::UpdateCamera (float DeltaTime) {
	// Could modify CameraRotationTarget and CameraLocationTarget before it's ultimately being used here
}
void ATBSPlayerController::ApplyCamera (float DeltaTime) {
	PlayerCharacter->CameraBoom->SetRelativeRotation (FMath::Lerp (PlayerCharacter->CameraBoom->RelativeRotation - FRotator (0, 180, 0), CameraRotationTarget, PlayerCharacter->CameraRotationLerpIntensity));
	PlayerCharacter->CameraBoom->AddRelativeRotation (FRotator (0, 180, 0));
}
#pragma endregion

#pragma region Pitch Hacks
void ATBSPlayerController::SpawnNextCustomer () {
	SpawnedNextCustomer();
	if (PlayerCharacter) {
		PlayerCharacter->LoadNewCustomer ();
	}
}

void ATBSPlayerController::FinishCurrentCustomer() {
	FinishedCurrentCustomer();
	if (PlayerCharacter) {
		PlayerCharacter->FinishCurrentCustomer();
	}
}
#pragma endregion

#pragma region Lift
void ATBSPlayerController::LiftPositionPressed() {
	((ATBSCharacter*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->CurrentCustomer->LiftPositionPressed();
}

void ATBSPlayerController::LiftPositionReleased() {
	((ATBSCharacter*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->CurrentCustomer->LiftPositionReleased();
}
#pragma endregion

#pragma region Beard Data Management

bool ATBSPlayerController::ClearBeardData() {
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

bool ATBSPlayerController::ClearBeardID (FName BeardName) {
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

bool ATBSPlayerController::LoadBeardID (FName BeardName) {
	if (PlayerCharacter) {
		UDataTable* DataTable;
		DataTable = FindDataTableToName (BeardName);
		if (DataTable) {
			return LoadBeardDataToCurrentCustomer (DataTable);
		}
	}
	return false;
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

UDataTable* ATBSPlayerController::FindDataTableToName (FName BeardName) {
	if (PlayerCharacter) {
		FBeardCollectionData* CurrentData;
		FName DataTableName;
		if (PlayerCharacter->BeardCollection) {
			const FString Context;
			for (int32 i = 0; i < PlayerCharacter->BeardCollection->GetRowNames().Num(); i++) {
				FName Row = PlayerCharacter->BeardCollection->GetRowNames()[i];
				CurrentData = PlayerCharacter->BeardCollection->FindRow<FBeardCollectionData> (Row, Context, false);
				if (CurrentData) {
					if (CurrentData->BeardName == BeardName) {
						DataTableName = CurrentData->BeardSlotName;
						for (int32 i = 0; i < PlayerCharacter->BeardData.Num (); i++) {
							if (PlayerCharacter->BeardData[i]->GetName() == DataTableName.ToString()) {
								return PlayerCharacter->BeardData[i];
								break;
							}
						}
						break;
					}
				}
				else {
					UE_LOG (LogClass, Warning, TEXT ("*** Could not find Beard in CollectionData! ***"));
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