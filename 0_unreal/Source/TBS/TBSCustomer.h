// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "TBSCustomer.generated.h"

USTRUCT (BlueprintType)
struct FBeardMeshData : public FTableRowBase {
	GENERATED_USTRUCT_BODY ()

public:

	FBeardMeshData ()
		: TranslateX (0)
		, TranslateY (0)
		, TranslateZ (0)
		, Roll (0)
		, Pitch (0)
		, Yaw (0)
	{}

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateX;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateY;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateZ;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Rotation")		float Roll;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Rotation")		float Pitch;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Rotation")		float Yaw;
};

USTRUCT(BlueprintType)
struct FBeardPoolData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	FBeardPoolData()
		: PlayerLevel(0)
		, BeardMinNumber(1)
		, BeardMaxNumber(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardPool")	int32 PlayerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardPool")	int32 BeardMinNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardPool")	int32 BeardMaxNumber;
};

UCLASS()
class TBS_API ATBSCustomer : public AActor {
	GENERATED_BODY ()

public:
	// Sets default values for this actor's properties
	ATBSCustomer (const FObjectInitializer& ObjectInitializer);

	UPROPERTY (EditAnywhere, noclear, BlueprintReadWrite, Category = "Beard", meta = (DisplayName = "Beard Class"))
	TSubclassOf<class AActor> BeardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beard")
	FName DesiredBeard;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Beard")
	AActor* Beard;

	UFUNCTION (BlueprintImplementableEvent, Category = "Beard")
	void CreatedCustomer ();
	void CreateNewCustomer(int32 CharacterLevel = 1);
	void CreateRandomDesiredBeard(int32 MaxLevelBeard);
	void FindDesiredBeardFromPool(int32 Playerlevel);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetupInputComponent();

	// Lifting of seat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
	float LiftUpSteps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
	float LiftDownSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
	FVector2D LiftLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
	float LiftPositionTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
	float LiftPositionCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lift")
	float LiftPositionLerpSpeed;

	FVector BeardStartPosition;
	FVector CustomerStartPosition;
	float LiftLastPressed;
	float LiftTimeForHold;
	UFUNCTION(BlueprintCallable, Category = "Lift")
	void LiftPositionPressed();
	UFUNCTION(BlueprintCallable, Category = "Lift")
	void LiftPositionReleased();

	UFUNCTION(BlueprintCallable, Category = "Lift")
	void LiftPositionApply();
	UFUNCTION(BlueprintCallable, Category = "Lift")
	void LiftPositionUpdate(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Lift")
	void LiftPositionUp();
	UFUNCTION(BlueprintCallable, Category = "Lift")
	void LiftPositionDown(float DeltaTime);
};
