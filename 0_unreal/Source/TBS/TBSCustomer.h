// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TBSCustomer.generated.h"

USTRUCT(BlueprintType)
struct FBeardMeshData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FBeardMeshData()
		: TranslateX(0)
		, TranslateY(0)
		, TranslateZ(0)
		, RotateX(0)
		, RotateY(0)
		, RotateZ(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")	float RotateX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")	float RotateY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")	float RotateZ;
};

UCLASS()
class TBS_API ATBSCustomer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBSCustomer(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Beard", meta = (DisplayName = "Beard Class"))
		TSubclassOf<class AActor> BeardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beard") AActor* Beard;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Beard")	void CreatedCustomer();

	void CreateNewCustomer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
};
