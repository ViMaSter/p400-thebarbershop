// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/SceneCaptureComponent2D.h"

#include "TBSBeard.h"
#include "TBSCustomer.generated.h"

UCLASS()
class TBS_API ATBSCustomer : public AActor {
	GENERATED_BODY ()

public:
	// Sets default values for this actor's properties
	ATBSCustomer(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Beard", meta = (DisplayName = "Beard Class"))
	TSubclassOf<class AActor> BeardClass;

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Screenshot Helper")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Screenshot Helper")
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beard")
	FName DesiredBeard;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Beard")
	AActor* Beard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutted Hairs")
	TArray<UActorComponent*> HairsCutted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial helper")
	TArray<int32> ForcedSessionBeards;

	UFUNCTION (BlueprintImplementableEvent, Category = "Beard")
	void CreatedCustomer ();
	void CreateNewCustomer(int32 CharacterLevel = 1);
	void CreateRandomDesiredBeard(int32 MaxLevelBeard);
	void FindDesiredBeardFromPool(int32 Playerlevel);

	FTimerHandle SpawnTimerHandle;
	int32 HairIndex = 0;
	void SpawnBeardPart();
	void FinisheBeardSpawning();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current Customer")
	bool IsCurrentCustomer = false;

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
