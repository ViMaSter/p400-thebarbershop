// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"

#include "TBSEquipment.h"
#include "TBSRazor.h"
#include "TBSCustomer.h"
#include "TBSCharacter.generated.h"



USTRUCT (BlueprintType)
struct FLevelUpData : public FTableRowBase {
	GENERATED_USTRUCT_BODY ()

public:
	FLevelUpData ()
		: Level (0)
		, XPtoLvl (0)
	{}

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 Level;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 XPtoLvl;
};

USTRUCT(BlueprintType)
struct FTimeBonusData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FTimeBonusData()
		: TimeMin(0)
		, TimeMax(0)
		, BonusCash(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonusCash")
		int32 TimeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonusCash")
		int32 TimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonusCash")
		int32 BonusCash;
};

USTRUCT (BlueprintType)
struct FBeardComparisonData : public FTableRowBase {
	GENERATED_USTRUCT_BODY ()

public:

	FBeardComparisonData ()
		: HairState (0)
	{}

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "HairInfo")
	int32 HairState;
};

USTRUCT (BlueprintType)
struct FBeardCollectionData : public FTableRowBase {
	GENERATED_USTRUCT_BODY ()

public:

	FBeardCollectionData()
		: BeardName("")
		, BeardSlotName("")
		, BeardLevel(0)
		, UniqueIdentifier(0)
	{}

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
	FName BeardName;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
	FName BeardSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
	int32 BeardLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
	int32 UniqueIdentifier;
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

	UPROPERTY (EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Customer Class"))
	TSubclassOf<ATBSCustomer> CustomerClass;

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

	// Timer
	FTimerHandle TimerHandle;
	float TimeLimit;


	// Customer
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	TArray<UDataTable*> BeardData;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	UDataTable* BeardCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data")
	TArray<UDataTable*> RedoUndoData;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* CurrentCustomer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* NextCustomer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* FirstCustomer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* SecondCustomer;

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
	
	void SwitchTool (bool IsNextTool);
	UFUNCTION(BlueprintCallable, Category = "EXP") float CalculateResult();
	void CalculateBonusCash();

	bool IsInTimeRange(float Time, int32 MinTime, int32 MaxTime);


#pragma region SessionData
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
	UFUNCTION(BlueprintCallable, Category = "Equipment")	bool BuyEquipment(int32 ID);

	TMap<int32, FTBSEquipmentData> GetEquipmentList();
	// Wrapper function, since TMap is not supported by the reflection system
	UFUNCTION(BlueprintCallable, Category = "Equipment")	FTBSEquipmentData GetEquipmentByID(int32 ID);
	UFUNCTION(BlueprintCallable, Category = "Equipment")	TArray<FTBSEquipmentData> GetEquipmentListAsArray();
#pragma endregion Equipment

#pragma region Multi-Threading
	bool CompLoadingStarted_MT = false;
	bool BonusLoadingStarted_MT = false;
	bool EquipmentLodingStarted_MT = false;
	bool LevelLodingStarted_MT = false;

	TArray<FBeardComparisonData*> BeardData_MT;
	TArray<FTimeBonusData*> TimeBonusData_MT;
	TArray<FTBSEquipmentData*> EquipmentData_MT;
	TArray<FLevelUpData*> LevelData_MT;

	FTimerHandle  CompLoadingTimeHandle_MT;
	FTimerHandle BonusLoadingTimeHandle_MT;
	FTimerHandle EquipmentLoadingTimeHandle_MT;
	FTimerHandle LevelLoadingTimeHandle_MT;
#pragma endregion Multi-Threading

	bool FirstCustomerActive = true;
};


