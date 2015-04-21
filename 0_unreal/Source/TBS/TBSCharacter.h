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

UCLASS(Blueprintable)
class ATBSCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Made these public so they can be accessed more easily
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tool, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ToolResetPosition;

	ATBSCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Tool Class"))
		TSubclassOf<class ATBSRazor> ToolClass;

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Tool Class"))
		TSubclassOf<class ATBSCustomer> CustomerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool")
		ATBSRazor* Tool;

	// Camera Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") int32 HorizontalCameraRotationBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") int32 VerticalUpperCameraRotationBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") int32 VerticalLowerCameraRotationBorder;
	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") ATBSCustomer* CurrentCustomer;

	void FinishCurrentCustomer();

	UFUNCTION(BlueprintCallable, Category="Gameplay|Timer")
	float GetTimeLeft();

	void SwitchTool(bool IsNextTool);
	UFUNCTION(BlueprintCallable, Category = "EXP")
		void IncreaseEXP(int32 Value);

	virtual void BeginPlay() override;
	void LoadNewCustomer();

	float CalculateResult();
};


