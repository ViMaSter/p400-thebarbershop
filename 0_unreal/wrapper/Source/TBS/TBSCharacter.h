// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "Engine/TextureRenderTarget.h"

#include "TBSEquipment.h"
#include "TBSRazor.h"
#include "TBSCustomer.h"
#include "TBSBeard.h"
#include "TBSProgression.h"
#include "TBSDataLoadWorker.h"
#include "TBSCharacter.generated.h"

USTRUCT()
struct FMTTask {
	GENERATED_USTRUCT_BODY()
	
		FTimerHandle Handle;
		FTBSDataLoadWorker* Runnable;
		bool TaskStarted = false;

		bool operator==(FMTTask RHT) {
			return this->Runnable == RHT.Runnable;
		}
};

UCLASS(Blueprintable)
class ATBSCharacter : public APawn {
	GENERATED_BODY ()

public:
	// Components
	// Camera
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;


	// Movement
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* CharacterMovement;


	// Tool
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Tool, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ToolResetPosition;

	UPROPERTY (EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Tool Class"))
	TSubclassOf<ATBSRazor> ToolClass;

	UPROPERTY (EditAnywhere, noclear, BlueprintReadWrite, Category = "Customer Class", meta = (DisplayName = "Customer Class"))
	TSubclassOf<ATBSCustomer> CustomerClass;
	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Customer Class", meta = (DisplayName = "ScreenCaptureCustomerClass"))
	TSubclassOf<ATBSCustomer> ScreenCaptureCustomerClass;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
	ATBSRazor* Tool;


	// Members
	// Camera Control
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float CameraRotationLerpIntensity;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Camera")
	int32 HorizontalCameraRotationBorder;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Camera")
	int32 VerticalUpperCameraRotationBorder;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Camera")
	int32 VerticalLowerCameraRotationBorder;


	// Player Status	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	UDataTable* LevelData;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	UDataTable* BeardPoolData;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	int32 CurrentLevel;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	int32 CurrentExperience;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	int32 CurrentExperienceToLvl;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	int32 CurrentCash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	UDataTable* BonusCashData;

	// Game Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Status")
	bool GameIsRunning = false;

	// Timer
	FTimerHandle TimerHandle;
	float TimeLimit;


	// Customer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	TArray<UDataTable*> BeardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	TArray<UTexture*> BeardIcons;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	UDataTable* BeardCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	TArray<UDataTable*> RedoUndoData;

	// Current beard rows
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	FBeardCollectionData CurrentBeardRow;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* CurrentCustomer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* NextCustomer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* FirstCustomer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* SecondCustomer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* ScreenCaptureCustomer;

	UFUNCTION(BlueprintCallable, Category = "PlayerStatus") FName GetDesiredCustomerBeard();


	// Constructor
	ATBSCharacter (const FObjectInitializer& ObjectInitializer);

	// UE4-Execution Events
	virtual void BeginPlay () override;
	virtual void Tick(float DeltaTime) override;

	// (BP-)Helper functions
	UFUNCTION (BlueprintCallable, Category = "Timer") float GetTimeElapsed();
	UFUNCTION (BlueprintCallable, Category = "Timer") float GetTimeLeft();
	UFUNCTION (BlueprintCallable, Category = "Timer") void PauseGameTimer();
	UFUNCTION (BlueprintCallable, Category = "Timer") void UnpauseGameTimer();
	UFUNCTION (BlueprintCallable, Category = "Timer") void ToggleGameTimer();

	UFUNCTION (BlueprintCallable, Category = "EXP") void IncreaseEXP(int32 Value);
	UFUNCTION (BlueprintCallable, Category = "Cash") void IncreaseCash(float ComparisionResult);
	UFUNCTION (BlueprintCallable, Category = "Customer") void LoadNewCustomer();
	UFUNCTION (exec, BlueprintCallable, Category = "Customer") void TransitionToNewCustomer();
	UFUNCTION (BlueprintCallable, Category = "Customer") void FinishCurrentCustomer();
	UFUNCTION (BlueprintCallable, Category = "Customer") void StartGame();
	
	void SwitchTool (bool IsNextTool);
	UFUNCTION(BlueprintCallable, Category = "EXP") float CalculateResult();
	void CalculateBonusCash();

	bool IsInTimeRange(float Time, int32 MinTime, int32 MaxTime);


#pragma region SessionData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	float LastBeardResult = 0;


private:
	uint32 SessionID = 0;
	float BeardResult = 0;

	void SaveSessionData();
#pragma endregion SessionData

#pragma region Equipment
public:
	// Event for Equipping an Item
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")	void EquipedItem(int32 ID);

	// EquipmentData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")	UDataTable* EquipmentData;

	UFUNCTION(BlueprintCallable, Category = "Equipment")	TArray<int32> GetObtainedEquipment();
	UFUNCTION(BlueprintCallable, Category = "Equipment")	bool EquipItem(int32 ID);
	UFUNCTION(BlueprintCallable, exec, Category = "Equipment")	bool BuyEquipment(int32 ID);

	TMap<int32, FTBSEquipmentData> GetEquipmentList();
	// Wrapper function, since TMap is not supported by the reflection system
	UFUNCTION(BlueprintCallable, Category = "Equipment")	FTBSEquipmentData GetEquipmentByID(int32 ID);
	UFUNCTION(BlueprintCallable, Category = "Equipment")	TArray<FTBSEquipmentData> GetEquipmentListAsArray();
#pragma endregion Equipment

#pragma region Multi-Threading
	void CheckMTTasks();
	void StartMTTasks(TArray<TSubclassOf<FTableRowBase>*>* OUT_Data, UDataTable* DataTable);

	TArray<FMTTask> MTTasks;
	
	TArray<FBeardComparisonData*> BeardData_MT;
	TArray<FTimeBonusData*> TimeBonusData_MT;
	TArray<FTBSEquipmentData*> EquipmentData_MT;
	TArray<FLevelUpData*> LevelData_MT;

#pragma endregion Multi-Threading

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customer management") bool FirstCustomerActive = true;

#pragma region MS2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop") UDataTable* MSWorkaround;
#pragma endregion

	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")	void SwapedScreenCap();
	UFUNCTION(BlueprintImplementableEvent, Category = "ScreenCap")	void ResultOpened();

};


