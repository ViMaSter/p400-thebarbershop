// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "TBSCharacter.h"
#include "Engine/LocalPlayer.h"
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
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool RedoBeardChanges();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool UndoBeardChanges();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool SaveStep();
	UFUNCTION (BlueprintCallable, Category = "Beard Control") void SetChangedBeard();

	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool ClearBeardData ();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool ClearBeardID (FName BeardName);
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool SaveBeardID(FName BeardName, int32 BeardLevel = 1, int32 UniqueId = 0);
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") bool LoadBeardID (FName BeardName);
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") void SpawnNextCustomer();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") void FinishCurrentCustomer();
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") UDataTable* FindDataTableToName(FName BeardName);
	UFUNCTION (BlueprintCallable, exec, Category = "Beard Control") TArray<FBeardNameLevelData> GetBeardNameLevelData();
	UFUNCTION (BlueprintImplementableEvent, Category = "Beard Control") void SpawnedNextCustomer();
	UFUNCTION (BlueprintImplementableEvent, Category = "Beard Control") void FinishedCurrentCustomer();

	UFUNCTION (BlueprintCallable, exec, Category = "Game Pause") void PauseGame();
	UFUNCTION (BlueprintCallable, exec, Category = "Game Pause") void UnpauseGame();
	UFUNCTION (BlueprintCallable, exec, Category = "Game Pause") void TogglePause();
	UFUNCTION (BlueprintCallable, Category = "Game Pause") bool CheckPaused();

	UFUNCTION (BlueprintCallable, exec, Category = "Game Mode") void SetEditorMode(bool EditorModeActive);
	UFUNCTION (BlueprintCallable, Category = "Game Mode") bool GetEditorMode();


	//BeginPlay
	virtual void BeginPlay () override;

protected:
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool InputIgnore;


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

	void RotateCamera (float Pitch, float Yaw);

	// Methods used in tick
	// Update: Prepares the target values
	// Apply: Checks whether or not these values should be used and applies them
	void UpdateRazor (float DeltaTime);
	void ApplyRazor (float DeltaTime);

	void UpdateCamera (float DeltaTime);
	void ApplyCamera (float DeltaTime);
	
	bool RemoveBeardFromCollection (FName BeardName);
	bool LoadBeardDataToCurrentCustomer(UDataTable* Datatable);
	bool SetCurrentBeardDataToCSV (UDataTable* DataTable);
	bool SetBeardToCollectionData(FName BeardName, int32 BeardLevel, int32 UniqueId);


	bool ChangedBeard = false;
	int32 StepIndex = -1;			// Starting with -1 cause initial SaveStep jumping to 0
	int32 TotalSteps = 0;
	int32 TotalUndoedSteps = 0;
	void InputRedoBeardChanges();
	void InputUndoBeardChanges();

};


