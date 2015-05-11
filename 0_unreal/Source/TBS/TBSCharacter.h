// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
#include "Engine/DataTable.h"

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

	UPROPERTY (EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Tool Class"))
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

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus")
	ATBSCustomer* CurrentCustomer;

	UFUNCTION(BlueprintCallable, Category = "PlayerStatus") FName GetDesiredCustomerBeard();


	// Constructor
	ATBSCharacter (const FObjectInitializer& ObjectInitializer);

	// UE4-Execution Events
	virtual void BeginPlay () override;

	// (BP-)Helper functions
	UFUNCTION (BlueprintCallable, Category = "Timer") float GetTimeElapsed();
	UFUNCTION (BlueprintCallable, Category = "Timer") float GetTimeLeft();
	UFUNCTION (BlueprintCallable, Category = "EXP") void IncreaseEXP(int32 Value);
	UFUNCTION (BlueprintCallable, Category = "Cash") void IncreaseCash(float ComparisionResult);
	UFUNCTION (BlueprintCallable, Category = "Customer") void LoadNewCustomer();
	UFUNCTION (BlueprintCallable, Category = "Customer") void FinishCurrentCustomer();
	
	void SwitchTool (bool IsNextTool);
	float CalculateResult ();
	void CalculateBonusCash();

	bool IsInTimeRange(float Time, int32 MinTime, int32 MaxTime);
};


