// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "TBSCustomer.h"
#include "TBSCharacter.h"
#include "Engine/LocalPlayer.h"
#include "TBSPlayerController.generated.h"

UCLASS()
class ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);

	/// Vincent: Needed to move this from protected to public, since this methods will be called from the HUD
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void ClearBeardID(FString BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void SaveBeardID(FString BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void LoadBeardID(FString BeardName);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") bool InputIgnore;

	// Smoothing of the razor when moving accross the face - higher value, more "snappyness"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float RazorRotationLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float RazorPositionLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float RazorLoweringLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float ShavingThreshold;

	FRotator CurrentToolRotation;
	FVector CurrentToolLocation;
	FVector CurrentToolHeightOffset;
	FRotator ToolRotation;
	FVector MouseCursorImpactNormal;
	FVector MouseCursorImpactPoint;

	bool ShaveActive;
	bool RotationActive;
	FVector2D StoredMousePosition;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void RotateTop(float Value);
	void RotateRight(float Value);
	void RotateToolTop(float Value);
	void RotateToolRight(float Value);
	void SwitchToNextTool();
	void SwitchToPrevTool();
	void UpdateRazorPosition(float DeltaTime);
	void OnSetShavedPressed();
	void OnSetShavedReleased();
	void OnSetRotationPressed();
	void OnSetRotationReleased();

	// Beard Control
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void SpawnNextCustomer();
	void LoadBeardDataToCurrentCustomer(UDataTable* Datatable);
	void SetCurrentBeardDataToCSV(UDataTable* DataTable);

};


