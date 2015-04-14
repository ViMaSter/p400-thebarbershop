// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSPlayerController.h"
#include "TBSCharacter.h"
#include "Engine/LocalPlayer.h"

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
	InputComponent->BindAxis("RotateRazorTop", this, &ATBSPlayerController::RotateRazorTop);
	//InputComponent->BindAxis("RotateRazorRight", this, &ATBSPlayerController::RotateRazorRight);

	InputComponent->BindAction("SwitchToNextTool", IE_Pressed, this, &ATBSPlayerController::SwitchToNextTool);
	InputComponent->BindAction("SwitchToPrevTool", IE_Pressed, this, &ATBSPlayerController::SwitchToPrevTool);
	InputComponent->BindAction("Shave", IE_Pressed, this, &ATBSPlayerController::OnSetShavedPressed);
	InputComponent->BindAction("Shave", IE_Released, this, &ATBSPlayerController::OnSetShavedReleased);
	InputComponent->BindAction("Rotate", IE_Pressed, this, &ATBSPlayerController::OnSetRotationPressed);
	InputComponent->BindAction("Rotate", IE_Released, this, &ATBSPlayerController::OnSetRotationReleased);
}

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
		if ((int32)CameraRotation.Yaw == -1*PlayerCharacter->HorizontalCameraRotationBorder && Value > 0 ||
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

void ATBSPlayerController::RotateRazorTop(float Value){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator ToolRotation;
	if (PlayerCharacter && RotationActive && Value != 0.f){
		Value *= -10;
		ToolRotation = PlayerCharacter->Razor->GetActorRotation();
		PlayerCharacter->Razor->AddActorLocalRotation(FRotator(0, 0, Value));

		ToolRotation.Roll = PlayerCharacter->Razor->GetActorRotation().Roll;
		PlayerCharacter->Razor->SetActorRotation(ToolRotation);
	}
}

void ATBSPlayerController::RotateRazorRight(float Value){
	ATBSCharacter* PlayerCharacter = (ATBSCharacter*)GetPawn();
	FRotator ToolRotation;
	if (PlayerCharacter && RotationActive && Value != 0.f){
		Value *= 10;
		ToolRotation = PlayerCharacter->Razor->GetActorRotation();
		PlayerCharacter->Razor->AddActorLocalRotation(FRotator(0, Value, 0));
		
		ToolRotation.Yaw = PlayerCharacter->Razor->GetActorRotation().Yaw;
		PlayerCharacter->Razor->SetActorRotation(ToolRotation);
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
		FRotator ToolRotation;
		if (Hitresult.GetActor()){
			if (ShaveActive){
				PlayerCharacter->Razor->SetActorLocation(Hitresult.ImpactPoint);
				PlayerCharacter->Razor->IsActive = true;
			}
			else{
				PlayerCharacter->Razor->SetActorLocation(Hitresult.ImpactPoint + Hitresult.ImpactNormal*PlayerCharacter->Razor->RazorHightInactive);
				PlayerCharacter->Razor->IsActive = false;
			}
			ToolRotation = PlayerCharacter->Razor->GetActorRotation();		
			ToolRotation.Pitch = Hitresult.ImpactNormal.Rotation().Pitch;
			ToolRotation.Yaw = Hitresult.ImpactNormal.Rotation().Yaw-180;

			PlayerCharacter->Razor->SetActorRotation(ToolRotation);
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