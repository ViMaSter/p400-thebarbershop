// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSPlayerController.h"

ATBSPlayerController::ATBSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = false;
	ShaveActive = false;
	RotationActive = false;

	PointingAtCustomer = false;

	ToolRotationTarget = FRotator(0, 0, 180);

	RazorRotationLerpIntensity = 1.0f;
	RazorPositionLerpIntensity = 1.0f;
	RazorLoweringLerpIntensity = 1.0f;
	ShavingThreshold = 0.1f;
}

void ATBSPlayerController::BeginPlay()
{
	PlayerCharacter = (ATBSCharacter*)GetPawn();
}

void ATBSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	UpdateRazor(DeltaTime);
	UpdateCamera(DeltaTime);

	ApplyRazor(DeltaTime);
	ApplyCamera(DeltaTime);
}

void ATBSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("RotateTop", this, &ATBSPlayerController::RotateTop);
	InputComponent->BindAxis("RotateRight", this, &ATBSPlayerController::RotateRight);
	InputComponent->BindAxis("RotateRazorTop", this, &ATBSPlayerController::RotateToolTop);

	InputComponent->BindAction("SwitchToNextTool", IE_Pressed, this, &ATBSPlayerController::SwitchToNextTool);
	InputComponent->BindAction("SwitchToPrevTool", IE_Pressed, this, &ATBSPlayerController::SwitchToPrevTool);
	InputComponent->BindAction("Shave", IE_Pressed, this, &ATBSPlayerController::OnSetShavedPressed);
	InputComponent->BindAction("Shave", IE_Released, this, &ATBSPlayerController::OnSetShavedReleased);
	InputComponent->BindAction("Rotate", IE_Pressed, this, &ATBSPlayerController::OnSetRotationPressed);
	InputComponent->BindAction("Rotate", IE_Released, this, &ATBSPlayerController::OnSetRotationReleased);

	// Cheat Codes Pitch Hack
	InputComponent->BindAction("SpawnNextCustomer", IE_Pressed, this, &ATBSPlayerController::SpawnNextCustomer);

}

#pragma region Camera Control
void ATBSPlayerController::RotateCamera(float Pitch, float Yaw)
{
	if (PlayerCharacter)
	{
		CameraRotationTarget.Add(Pitch, Yaw, 0);

		CameraRotationTarget.Pitch = FMath::ClampAngle(CameraRotationTarget.Pitch, PlayerCharacter->VerticalUpperCameraRotationBorder, PlayerCharacter->VerticalLowerCameraRotationBorder);
		CameraRotationTarget.Yaw = FMath::ClampAngle(CameraRotationTarget.Yaw, -PlayerCharacter->HorizontalCameraRotationBorder, PlayerCharacter->HorizontalCameraRotationBorder);
	}
}

void ATBSPlayerController::RotateTop(float Value)
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	RotateCamera(Value, 0);
}

void ATBSPlayerController::RotateRight(float Value)
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	RotateCamera(0, -Value);
}
#pragma endregion

#pragma region Tool Control
void ATBSPlayerController::RotateToolTop(float Value)
{
	if (RotationActive && Value != 0.f)
	{
		Value *= -10;
		PlayerCharacter->Tool->AddActorLocalRotation(FRotator(0, 0, Value));
	}
}

// Deprecated
// Note: Want confirmation - just remove it and commit that, if realy unnecessary
void ATBSPlayerController::RotateToolRight(float Value)
{
	if (RotationActive && Value != 0.f)
	{
		Value *= 10;
		PlayerCharacter->Tool->AddActorLocalRotation(FRotator(0, Value, 0));
	}
}


void ATBSPlayerController::OnSetShavedPressed()
{
	ShaveActive = true;
}

void ATBSPlayerController::OnSetShavedReleased()
{
	ShaveActive = false;
}

void ATBSPlayerController::OnSetRotationPressed()
{
	RotationActive = true;
	
	GetMousePosition(StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::OnSetRotationReleased()
{
	ToolRotationTarget = PlayerCharacter->Tool->GetActorRotation();

	RotationActive = false;
	ULocalPlayer* LocalPlayer = CastChecked<ULocalPlayer>(Player);
	FViewport* ViewPort = LocalPlayer->ViewportClient->Viewport;
	ViewPort->SetMouse(StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::SwitchToNextTool()
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	PlayerCharacter->SwitchTool(true);
}

void ATBSPlayerController::SwitchToPrevTool()
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	PlayerCharacter->SwitchTool(false);
}
#pragma endregion

#pragma region Tick
void ATBSPlayerController::UpdateRazor(float DeltaTime)
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	if (PlayerCharacter && !RotationActive)
	{
		// Fetch current values
		ToolRotationCurrent = PlayerCharacter->Tool->GetActorRotation();
		ToolLocationCurrent = PlayerCharacter->Tool->GetActorLocation();

		// Get hit from mouse cursor ray
		FHitResult Hitresult;
		GetHitResultUnderCursor(ECC_WorldDynamic, true, Hitresult);

		if (Hitresult.GetActor() && Hitresult.GetActor()->GetClass()->IsChildOf(ATBSCustomer::StaticClass()))
		{
			// Save hitresult info
			LastValidMouseCursorImpactPoint = Hitresult.ImpactPoint;
			LastValidMouseCursorImpactNormal = Hitresult.ImpactNormal;

			// SKIN OFFSET
			// Lower/raise the tool based on ::ShaveActive
			if (ShaveActive)
			{
				ToolHeightOffsetTarget = FVector::ZeroVector;
			}
			else
			{
				ToolHeightOffsetTarget = Hitresult.ImpactNormal*PlayerCharacter->Tool->ToolInactiveHight;
			}
			ToolHeightOffsetCurrent = FMath::Lerp(ToolHeightOffsetCurrent, ToolHeightOffsetTarget, (1.0f / DeltaTime / 60.0f) * RazorLoweringLerpIntensity);

			// Whether or not the tool is active, is now dependent on the razors distance from the skin
			PlayerCharacter->Tool->IsActive = ToolHeightOffsetCurrent.Size() < ShavingThreshold;

			// POSITION
			ToolLocationTarget = Hitresult.ImpactPoint + ToolHeightOffsetCurrent;

			// ROTATION
			ToolRotationTarget.Pitch = Hitresult.ImpactNormal.Rotation().Pitch;
			ToolRotationTarget.Yaw = Hitresult.ImpactNormal.Rotation().Yaw - 180;

			PointingAtCustomer = true;
		}
		else
		{
			// Fall back to our 
			ToolLocationTarget = PlayerCharacter->ToolResetPosition->GetComponentLocation();
			ToolRotationTarget = PlayerCharacter->ToolResetPosition->GetComponentRotation();

			PointingAtCustomer = false;
		}
	}
}

void ATBSPlayerController::ApplyRazor(float DeltaTime)
{
	// Could handle special PointingAtCustomer-case here
	PlayerCharacter->Tool->SetActorLocation(
		FMath::Lerp(ToolLocationCurrent, ToolLocationTarget, (1.0f / DeltaTime / 60.0f) * RazorPositionLerpIntensity)
	);

	PlayerCharacter->Tool->SetActorRotation(
		FMath::Lerp(ToolRotationCurrent, ToolRotationTarget, (1.0f / DeltaTime / 60.0f) * RazorRotationLerpIntensity)
	);
}

void ATBSPlayerController::UpdateCamera(float DeltaTime)
{
	// Could modify CameraRotationTarget and CameraLocationTarget before it's ultimately being used here
}
void ATBSPlayerController::ApplyCamera(float DeltaTime)
{
	PlayerCharacter->GetCameraBoom()->SetRelativeRotation(FMath::Lerp(PlayerCharacter->GetCameraBoom()->RelativeRotation - FRotator(0, 180, 0), CameraRotationTarget, PlayerCharacter->CameraRotationLerpIntensity));
	PlayerCharacter->GetCameraBoom()->AddRelativeRotation(FRotator(0, 180, 0));
}
#pragma endregion

#pragma region Pitch Hacks
void ATBSPlayerController::SpawnNextCustomer(){
	if (PlayerCharacter){
		PlayerCharacter->LoadNewCustomer();
	}
}
#pragma endregion



#pragma region Beard Data Management
void ATBSPlayerController::ClearBeardID(FString BeardName)
{
	if (PlayerCharacter){
		for (int32 i = 0; i < PlayerCharacter->BeardData.Num(); i++){
			if (PlayerCharacter->BeardData[i]->GetName() == BeardName){
				PlayerCharacter->BeardData[i]->EmptyTable();
				break;
			}
		}
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}
/*
void ATBSPlayerController::ClearBeardID(FName BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FBeardComparisonData* CurrentData;
	if (PlayerCharacter && PlayerCharacter->BeardData){
		const FString Context;
		CurrentData = PlayerCharacter->BeardData->FindRow<FBeardComparisonData>(BeardName, Context);
		PlayerCharacter->BeardData->RowMap.Remove(BeardName);
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}
*/

void ATBSPlayerController::SaveBeardID(FString BeardName)
{
	if (PlayerCharacter){
		for (int32 i = 0; i < PlayerCharacter->BeardData.Num(); i++){
			if (PlayerCharacter->BeardData[i]->GetName() == BeardName){
				SetCurrentBeardDataToCSV(PlayerCharacter->BeardData[i]);
				break;
			}
		}

		// Saving still under development
		/*FString Path = FPaths::GameContentDir();
		Path.Append(TEXT("TheBarberShop/Data/"));
		Path.Append(BeardName);
		Path.Append(TEXT(".json"));
		*/
		//PlayerCharacter->BeardData->SaveConfig(16384Ui64, *Path);
		//UE_LOG(LogClass, Warning, TEXT("*** Saved Data to %s! ***"), *Path);
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}

void ATBSPlayerController::LoadBeardID(FString BeardName)
{
	if (PlayerCharacter){
		for (int32 i= 0; i < PlayerCharacter->BeardData.Num();i++){
			if (PlayerCharacter->BeardData[i]->GetName() == BeardName){
				LoadBeardDataToCurrentCustomer(PlayerCharacter->BeardData[i]);
				break;
			}
		}
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}

void ATBSPlayerController::SetCurrentBeardDataToCSV(UDataTable* DataTable)
{
	if (PlayerCharacter == NULL) return;
	TArray<UActorComponent*> Components;
	Components = PlayerCharacter->CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	FBeardComparisonData* CurrentData;
	for (int32 i = 0; i < Components.Num(); i++){
		int32 ComponentStatus;
		const FString Context;
		UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
		if (!Mesh->IsVisible()){
			ComponentStatus = 0;
		}
		else if (Mesh->GetCollisionResponseToChannel(ECC_Vehicle) == ECR_Ignore){
			ComponentStatus = 1;
		}
		else{
			ComponentStatus = 2;
		}
		FString String = FString::FromInt(i);
		FName Row = FName(*String);
		CurrentData = DataTable->FindRow<FBeardComparisonData>(Row, Context, false);

		if (CurrentData){
			CurrentData->HairState = ComponentStatus;
		}
		else{
			UScriptStruct* LoadUsingStruct = FBeardComparisonData::StaticStruct();
			uint8* RowData = (uint8*)FMemory::Malloc(LoadUsingStruct->PropertiesSize);
			DataTable->RowMap.Add(Row, RowData);
			CurrentData = DataTable->FindRow<FBeardComparisonData>(Row, Context, false);
			if (CurrentData){
				CurrentData->HairState = ComponentStatus;
			}
		}
	}
}

void ATBSPlayerController::LoadBeardDataToCurrentCustomer(UDataTable* DataTable)
{
	if (PlayerCharacter == NULL) return;
	TArray<UActorComponent*> Components;
	Components = PlayerCharacter->CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	FBeardComparisonData* CurrentData;
	const FString Context;
	for (int32 i = 0; i < Components.Num(); i++){
		FString String = FString::FromInt(i);
		FName Row = FName(*String);
		CurrentData = DataTable->FindRow<FBeardComparisonData>(Row, Context, false);
		if (CurrentData){
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
			switch (CurrentData->HairState)
			{
			case(0) :
				Mesh->SetVisibility(false);
				Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
				PlayerCharacter->Tool->Trimmed(1, Components[i]);
				break;
			case(1):
				Mesh->SetVisibility(true);
				Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
				PlayerCharacter->Tool->Trimmed(0.8, Components[i]);

				break;
			case(2):
				Mesh->SetVisibility(true);
				Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
				PlayerCharacter->Tool->Trimmed(0, Components[i]);
				break;
			}
		}
		else{
			UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard Data Row! Possible missmatch of Meshcount ***"));
			break;
		}
	}
}
#pragma endregion