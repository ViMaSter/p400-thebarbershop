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
	ATBSCustomer();

	UPROPERTY(EditAnywhere, noclear, BlueprintReadWrite, Category = "Beared", meta = (DisplayName = "Beared Class"))
		TSubclassOf<class AActor> BearedClass;

	UFUNCTION(BlueprintImplementableEvent, Category = "Beared")	void CreatedCustomer();

	void CreateNewCustomer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
};
