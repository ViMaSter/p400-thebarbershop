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


UCLASS(Blueprintable)
class ATBSCharacter : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementComponent* CharacterMovement;

public:

	ATBSCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Razor Class", meta = (DisplayName = "Razor Class"))
		TSubclassOf<class ATBSRazor> RazorClass;

	ATBSRazor* Razor;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStatus") ATBSCustomer* CurrentCustomer;

	void FinishCurrentCustomer();

	float GetTimeLeft();

	void SwitchTool(bool IsNextTool);
	void IncreaseEXP(int32 Value);

	virtual void BeginPlay() override;
	void LoadNewCustomer();
};


