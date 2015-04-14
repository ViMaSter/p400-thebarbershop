// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSCustomer.generated.h"

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
