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
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void ClearBeardID(FName BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void SaveBeardID(FName BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void LoadBeardID(FName BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") TArray<FName> GetBeardNames();
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void SpawnNextCustomer();

	//BeginPlay
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input") bool InputIgnore;

	// Smoothing of the razor when moving accross the face - higher value, more "snappyness"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float RazorRotationLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float RazorPositionLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float RazorLoweringLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float ShavingThreshold;

#pragma region References
	ATBSCharacter* PlayerCharacter;
#pragma endregion

#pragma region Camera
	FRotator CameraRotationTarget;
#pragma endregion

#pragma region Tool
	FVector ToolLocationCurrent;
	FVector ToolLocationTarget;

	FRotator ToolRotationCurrent;
	FRotator ToolRotationTarget;

	FVector ToolHeightOffsetCurrent;
	FVector ToolHeightOffsetTarget;
#pragma endregion

#pragma region MouseInfo
	// Currently not used for anything but could provide usefull
	FVector LastValidMouseCursorImpactNormal;
	FVector LastValidMouseCursorImpactPoint;
	FVector2D StoredMousePosition;

	bool PointingAtCustomer;

	bool ShaveActive;		// LMB
	bool RotationActive;	// RMB
#pragma endregion

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	// Methods used for input
	void RotateTop(float Value);
	void RotateRight(float Value);
	void RotateToolTop(float Value);
	void RotateToolRight(float Value);
	void SwitchToNextTool();
	void SwitchToPrevTool();
	void OnSetShavedPressed();
	void OnSetShavedReleased();
	void OnSetRotationPressed();
	void OnSetRotationReleased();

	void RotateCamera(float Pitch, float Yaw);

	// Methods used in tick
		// Update: Prepares the target values
		// Apply: Checks wether or not these values should be used and applies them
	void UpdateRazor(float DeltaTime);
	void ApplyRazor(float DeltaTime);

	void UpdateCamera(float DeltaTime);
	void ApplyCamera(float DeltaTime);

	UDataTable* FindDataTableToName(FName BeardName);
	void RemoveBeardFromCollection(FName BeardName);
	void LoadBeardDataToCurrentCustomer(UDataTable* Datatable);
	void SetCurrentBeardDataToCSV(UDataTable* DataTable); 
	void SetBeardToCollectionData(FName BeardName);


};


