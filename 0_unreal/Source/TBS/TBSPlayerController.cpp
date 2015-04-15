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

void ATBSPlayerController::RotateTop(float Value){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator CameraRotation;
	if (PlayerCharacter && Value != 0.f){
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

void ATBSPlayerController::RotateRight(float Value){
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

void ATBSPlayerController::SwitchToNextTool(){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter){
		PlayerCharacter->SwitchTool(true);
	}
}

void ATBSPlayerController::SwitchToPrevTool(){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter){
		PlayerCharacter->SwitchTool(false);
	}
}

void ATBSPlayerController::UpdateRazorPosition(){
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

#pragma region Beared Data Control

void ATBSPlayerController::ClearAllBeardData(){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FBeardComparisonData CurrentData;
	if (PlayerCharacter && PlayerCharacter->BeardData){
		PlayerCharacter->BeardData->EmptyTable();
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}

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

// Wrapper for ReplaceBeardId and AddBeardID
void ATBSPlayerController::SaveBeardID(FName BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FBeardComparisonData* CurrentData;
	if (PlayerCharacter && PlayerCharacter->BeardData){
		const FString Context;
		CurrentData = PlayerCharacter->BeardData->FindRow<FBeardComparisonData>(BeardName, Context);
		if (CurrentData){
			SetCurrentBeardDataToCSV(CurrentData);
		}
		else{
			UScriptStruct* LoadUsingStruct = FBeardComparisonData::StaticStruct();
			uint8* RowData = (uint8*)FMemory::Malloc(LoadUsingStruct->PropertiesSize);
			PlayerCharacter->BeardData->RowMap.Add(BeardName, RowData);
			if (CurrentData){
			}
			CurrentData = PlayerCharacter->BeardData->FindRow<FBeardComparisonData>(BeardName, Context);
			SetCurrentBeardDataToCSV(CurrentData);
			FString Path = FPaths::GameContentDir();
			Path.Append(TEXT("/TheBarberShop/Data/BeardComparisonData.uasset"));
			PlayerCharacter->BeardData->SaveConfig(16384Ui64,*Path);
		}
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}

void ATBSPlayerController::SetCurrentBeardDataToCSV(FBeardComparisonData* CurrentData){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	if (PlayerCharacter == NULL) return;
	if (CurrentData){
		FName BeardName = "Beard_1";
		const FString Context;		
		if (PlayerCharacter->CurrentCustomer && PlayerCharacter->CurrentCustomer->Beard){
			TArray<UActorComponent*> Components;
			int32 NumberShaved = 0;
			int32 NumberTrimmed = 0;
			int32 NumberNormal = 0;
			Components = PlayerCharacter->CurrentCustomer->Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
			for (int32 i = 0; i < Components.Num(); i++){
				UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
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
			CurrentData->Normal = NumberNormal;
			CurrentData->Trimmed = NumberTrimmed;
			CurrentData->Shaved = NumberShaved;
			CurrentData->Total = NumberShaved + NumberTrimmed + NumberNormal;
			UE_LOG(LogClass, Log, TEXT("*** Set the current Beard in CSV Beard Data ***"));
		}
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not Set Beard in CSV Data! Row in Beard CSV Data not found! ***"))
}


// Deprecated
void ATBSPlayerController::AddBeardID(FName BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FBeardComparisonData* CurrentData;
	if (PlayerCharacter && PlayerCharacter->BeardData){
		const FString Context;
		UScriptStruct* LoadUsingStruct = FBeardComparisonData::StaticStruct();
		uint8* RowData = (uint8*)FMemory::Malloc(LoadUsingStruct->PropertiesSize);
		PlayerCharacter->BeardData->RowMap.Add(BeardName, RowData);
		CurrentData = PlayerCharacter->BeardData->FindRow<FBeardComparisonData>(BeardName, Context);
		SetCurrentBeardDataToCSV(CurrentData);
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}

// Deprecated
void ATBSPlayerController::ReplaceBeardID(FName BeardName){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FBeardComparisonData* CurrentData;
	if (PlayerCharacter && PlayerCharacter->BeardData){
		const FString Context;
		CurrentData = PlayerCharacter->BeardData->FindRow<FBeardComparisonData>(BeardName, Context);
		SetCurrentBeardDataToCSV(CurrentData);
	}
	else UE_LOG(LogClass, Warning, TEXT("*** Could not load Beard CSV Data! ***"))
}

#pragma endregion