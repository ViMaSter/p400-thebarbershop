// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSPlayerController.h"

ATBSPlayerController::ATBSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = false;
	ShaveActive = false;
	RotationActive = false;
}

void ATBSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	UpdateRazorPosition();
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

void ATBSPlayerController::RotateTop(float Value)
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator CameraRotation;
	if (PlayerCharacter && Value != 0.f) {
		Value *= -1;

		CameraRotation = PlayerCharacter->GetCameraBoom()->RelativeRotation;
		if ((int32)CameraRotation.Pitch == PlayerCharacter->VerticalUpperCameraRotationBorder && Value < 0 ||
			(int32)CameraRotation.Pitch == PlayerCharacter->VerticalLowerCameraRotationBorder && Value > 0){
			return;
		}
		PlayerCharacter->GetCameraBoom()->AddRelativeRotation(FRotator(Value, 0, 0));
		CameraRotation.Roll = 0;
		CameraRotation.Pitch = PlayerCharacter->GetCameraBoom()->RelativeRotation.Pitch;

		PlayerCharacter->GetCameraBoom()->RelativeRotation = CameraRotation;
		PlayerCharacter->GetCameraBoom()->AddRelativeRotation(FRotator(0, 0, 0));
	}
}

void ATBSPlayerController::RotateRight(float Value)
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator CameraRotation;
	if (PlayerCharacter && Value != 0.f){
		Value *= -1;
		CameraRotation = PlayerCharacter->GetCameraBoom()->RelativeRotation;
		if ((int32)CameraRotation.Yaw == -1 * PlayerCharacter->HorizontalCameraRotationBorder && Value > 0 ||
			(int32)CameraRotation.Yaw == PlayerCharacter->HorizontalCameraRotationBorder && Value < 0){
			return;
		}
		PlayerCharacter->GetCameraBoom()->AddRelativeRotation(FRotator(0, Value, 0));
		CameraRotation.Roll = 0;
		CameraRotation.Yaw = PlayerCharacter->GetCameraBoom()->RelativeRotation.Yaw;

		PlayerCharacter->GetCameraBoom()->RelativeRotation = CameraRotation;
		PlayerCharacter->GetCameraBoom()->AddRelativeRotation(FRotator(0, 0, 0));
	}
}

#pragma endregion

#pragma region Tool Control

void ATBSPlayerController::RotateToolTop(float Value){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator ToolRotation;
	if (PlayerCharacter && RotationActive && Value != 0.f){
		Value *= -10;
		ToolRotation = PlayerCharacter->Tool->GetActorRotation();
		PlayerCharacter->Tool->AddActorLocalRotation(FRotator(0, 0, Value));

		ToolRotation.Roll = PlayerCharacter->Tool->GetActorRotation().Roll;
		PlayerCharacter->Tool->SetActorRotation(ToolRotation);
	}
}

// Deprecated
void ATBSPlayerController::RotateToolRight(float Value){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator ToolRotation;
	if (PlayerCharacter && RotationActive && Value != 0.f){
		Value *= 10;
		ToolRotation = PlayerCharacter->Tool->GetActorRotation();
		PlayerCharacter->Tool->AddActorLocalRotation(FRotator(0, Value, 0));

		ToolRotation.Yaw = PlayerCharacter->Tool->GetActorRotation().Yaw;
		PlayerCharacter->Tool->SetActorRotation(ToolRotation);
	}
}

void ATBSPlayerController::SwitchToNextTool()
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter){
		PlayerCharacter->SwitchTool(true);
	}
}

void ATBSPlayerController::SwitchToPrevTool()
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter){
		PlayerCharacter->SwitchTool(false);
	}
}

void ATBSPlayerController::UpdateRazorPosition()
{
	// TODO: Dirty Pitch-hack - REMOVE THIS FOR GOD SAKES
	if (InputIgnore)
	{
		return;
	}

	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter && !RotationActive){
		FHitResult Hitresult;
		GetHitResultUnderCursor(ECC_WorldDynamic, true, Hitresult);
		if (Hitresult.GetActor() && Hitresult.GetActor()->GetClass()->IsChildOf(ATBSCustomer::StaticClass())){
			if (ShaveActive){
				PlayerCharacter->Tool->SetActorLocation(Hitresult.ImpactPoint);
				PlayerCharacter->Tool->IsActive = true;
			}
			else{
				PlayerCharacter->Tool->SetActorLocation(Hitresult.ImpactPoint + Hitresult.ImpactNormal*PlayerCharacter->Tool->ToolInactiveHight);
				PlayerCharacter->Tool->IsActive = false;
			}
			FRotator ToolRotation;
			ToolRotation = PlayerCharacter->Tool->GetActorRotation();
			ToolRotation.Pitch = Hitresult.ImpactNormal.Rotation().Pitch;
			ToolRotation.Yaw = Hitresult.ImpactNormal.Rotation().Yaw - 180;

			PlayerCharacter->Tool->SetActorRotation(ToolRotation);
		}
	}
}



void ATBSPlayerController::OnSetShavedPressed(){
	ShaveActive = true;
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	PlayerCharacter->GetTimeLeft();
}

void ATBSPlayerController::OnSetShavedReleased(){
	ShaveActive = false;
}

void ATBSPlayerController::OnSetRotationPressed(){
	RotationActive = true;
	GetMousePosition(StoredMousePosition.X, StoredMousePosition.Y);
}

void ATBSPlayerController::OnSetRotationReleased(){
	RotationActive = false;
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	ULocalPlayer* LocalPlayer = CastChecked<ULocalPlayer>(Player);
	FViewport* ViewPort = LocalPlayer->ViewportClient->Viewport;
	ViewPort->SetMouse(StoredMousePosition.X, StoredMousePosition.Y);
}

#pragma endregion

#pragma region Pitch Hacks
void ATBSPlayerController::SpawnNextCustomer(){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter){
		PlayerCharacter->LoadNewCustomer();
	}
}

#pragma  endregion



#pragma region Beared Data Management

void ATBSPlayerController::ClearBeardID(FString BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
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

void ATBSPlayerController::SaveBeardID(FString BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
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

void ATBSPlayerController::LoadBeardID(FString BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
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

void ATBSPlayerController::SetCurrentBeardDataToCSV(UDataTable* DataTable){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
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

void ATBSPlayerController::LoadBeardDataToCurrentCustomer(UDataTable* DataTable){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
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