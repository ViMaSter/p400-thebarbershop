// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "TBSRazor.h"
#include "Engine/DataTable.h"
#include "TBSCustomer.h"
#include "TBSCharacter.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpData : public FTableRowBase{
	GENERATED_USTRUCT_BODY()

public:
	FLevelUpData()
		: Level(0)
		, XPtoLvl(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")		int32 XPtoLvl;
};

USTRUCT(BlueprintType)
struct FBeardComparisonData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FBeardComparisonData()
		:HairState(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairInfo")	int32 HairState;
};

USTRUCT(BlueprintType)
struct FBeardCollectionData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FBeardCollectionData()
		:BeardName("")
		,BeardSlotName("")
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection") FName BeardName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection") FName BeardSlotName;
};

UCLASS(Blueprintable)
class ATBSCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Components
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) class USpringArmComponent* CameraBoom;

	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) class UCharacterMovementComponent* CharacterMovement;

	// Tool
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tool, meta = (AllowPrivateAccess = "true")) class USceneComponent* ToolResetPosition;
	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Tool Class")) TSubclassOf<class ATBSRazor> ToolClass;
	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Tool Class")) TSubclassOf<class ATBSCustomer> CustomerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool") ATBSRazor* Tool;

	// Members
	// Camera Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juiciness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0")) float CameraRotationLerpIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") int32 HorizontalCameraRotationBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") int32 VerticalUpperCameraRotationBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") int32 VerticalLowerCameraRotationBorder;

	// Player Status	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data") UDataTable* LevelData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") int32 CurrentLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") int32 CurrentExperience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") int32 CurrentExperienceToLvl;

	// Timer
	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") float TimeLimit;
	
	// Customer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data") TArray<UDataTable*> BeardData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSV Data") UDataTable* BeardCollection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") ATBSCustomer* CurrentCustomer;

	// Constructor
	ATBSCharacter(const FObjectInitializer& ObjectInitializer);

	// UE4-Execution Events
	virtual void BeginPlay() override;

	// (BP-)Helper functions
	UFUNCTION(BlueprintCallable, Category = "Timer") float GetTimeLeft();
	UFUNCTION(BlueprintCallable, Category = "EXP") void IncreaseEXP(int32 Value);

	void FinishCurrentCustomer();
	void SwitchTool(bool IsNextTool);

	void LoadNewCustomer();

	float CalculateResult();
};


