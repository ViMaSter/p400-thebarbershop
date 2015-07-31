// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

#include "TBSCharacter.h"
#include "TBSPlayerController.generated.h"


#define MAXREDOSTEPS 10

USTRUCT(BlueprintType)
struct FBeardNameLevelData{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		FName BeardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		int32 BeardLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		int32 UniqueID;
};

UCLASS()
class ATBSPlayerController : public APlayerController {
	GENERATED_BODY ()

public:
	ATBSPlayerController (const FObjectInitializer& ObjectInitializer);

	/// Vincent: Needed to move this from protected to public, since this methods will be called from the HUD
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool SaveStep();
	UFUNCTION (BlueprintCallable, Category = "Beard Control") void SetChangedBeard();

	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool ClearBeardData ();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool ClearBeardID (FName BeardName);
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool SaveBeardID(FName BeardName, int32 BeardLevel = 1, int32 UniqueId = 0);
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool LoadBeardID (FName BeardName);

	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void LoadBeardID_MT(FName BeardName);

	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") void SpawnNextCustomer();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") void FinishCurrentCustomer();
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") FBeardCollectionData FindDataRowToName(FName BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") UDataTable* FindDataTableToName(FName BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") TArray<FBeardNameLevelData> GetBeardNameLevelData();
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") bool RedoBeardChanges();
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") bool UndoBeardChanges();
	UFUNCTION (BlueprintImplementableEvent, Category = "Beard Control") void SpawnedNextCustomer();
	UFUNCTION (BlueprintImplementableEvent, Category = "Beard Control") void FinishedCurrentCustomer();

#pragma region GameState Wrapper
	UFUNCTION(BlueprintCallable, exec, Category = "GameState Wrapper") bool GetIsPaused();
	UFUNCTION(BlueprintCallable, exec, Category = "GameState Wrapper") void SetIsPaused(bool IsPaused);

	UFUNCTION(BlueprintCallable, exec, Category = "GameState Wrapper") bool GetIsEditorMode();
	UFUNCTION(BlueprintCallable, exec, Category = "GameState Wrapper") void SetIsEditorMode(bool IsEditorMode);

	UFUNCTION(BlueprintCallable, exec, Category = "GameState Wrapper") bool GetIsIngame();
	UFUNCTION(BlueprintCallable, exec, Category = "GameState Wrapper") void SetIsIngame(bool IsIngame);
#pragma endregion

#pragma region SaveGame Wrapper
	UFUNCTION(BlueprintCallable, exec, Category = "SaveGame Wrapper") void UpdateCurrentSaveGame();
#pragma endregion

	//BeginPlay
	virtual void BeginPlay () override;

protected:
	// Smoothing of the razor when moving accross the face - higher value, more "snappyness"
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float RazorRotationLerpIntensity;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float RazorPositionLerpIntensity;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float RazorLoweringLerpIntensity;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ShavingThreshold;


#pragma region References
public:
	ATBSCharacter* PlayerCharacter;
#pragma endregion

#pragma region Camera
protected:
	FRotator CameraRotationTarget;
#pragma endregion

#pragma region Tool
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ToolLocationCurrent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ToolLocationTarget;

	FRotator ToolRotationCurrent;
	FRotator ToolRotationTarget;

	FVector ToolHeightOffsetCurrent;
	FVector ToolHeightOffsetTarget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsSweapMovement = false;
#pragma endregion

#pragma region Mechanics
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shave Mechanics")
	bool IsLegalShaveAngle = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shave Mechanics")
	float LegalShaveAngle = 45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shave Mechanics")
	bool IsMovingShave = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shave Mechanics")
	float MinimumShaveSpeed = 1.0f;

	void UpdateMechanicChecks();
#pragma endregion

#pragma region MouseInfo
protected:
	// Currently not used for anything but could provide usefull
	FVector LastValidMouseCursorImpactNormal;
	FVector LastValidMouseCursorImpactPoint;
	FVector2D StoredMousePosition;

	bool PointingAtCustomer;

	bool ShaveActive;		// LMB
	bool RotationActive;	// RMB
#pragma endregion

#pragma region Lift
	UFUNCTION(BlueprintCallable, Category = "Beard")
	void LiftPositionPressed();
	UFUNCTION(BlueprintCallable, Category = "Beard")
	void LiftPositionReleased();
#pragma endregion

	virtual void PlayerTick (float DeltaTime) override;
	virtual void SetupInputComponent () override;

	// Methods used for input
	void RotateTop (float Value);
	void RotateRight (float Value);
	void RotateToolTop (float Value);
	void SwitchToNextTool ();
	void SwitchToPrevTool ();
	void OnSetShavedPressed ();
	void OnSetShavedReleased ();
	void OnSetRotationPressed ();
	void OnSetRotationReleased ();

public:
	void ResetCamera();	
	void RotateCamera(float Pitch, float Yaw);

	// Methods used in tick
	// Update: Prepares the target values
	// Apply: Checks whether or not these values should be used and applies them
	void UpdateRazor(float DeltaTime);
	void ApplyRazor(float DeltaTime);

	void UpdateCamera(float DeltaTime);
	void ApplyCamera(float DeltaTime);

protected:
	bool RemoveBeardFromCollection (FName BeardName);
	bool LoadBeardDataToCurrentCustomer(UDataTable* Datatable);
	bool SetCurrentBeardDataToCSV (UDataTable* DataTable);
	bool SetBeardToCollectionData(FName BeardName, int32 BeardLevel, int32 UniqueId);

	bool InitialSaved = false;
	bool ChangedBeard = false;
	int32 StepIndex = -1;			// Starting with -1 cause initial SaveStep jumping to 0
	int32 TotalSteps = 0;
	int32 TotalUndoedSteps = 0;

	bool LoadingStarted_MT = false;
	TArray<FBeardComparisonData*> BeardData_MT;
	FTimerHandle LoadingTimeHandle_MT;

	void LoadBeardToCustomer(TArray<FBeardComparisonData*> Data);

	void SwitchToShortRazor();
	void SwitchToNormalRazor();
	void SwitchToClipper();

};