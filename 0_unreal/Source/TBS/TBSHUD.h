// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TBSHUDState.h"
#include "TBSHUD.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State") ETBSHUDState CurrentHUDState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State") ETBSHUDState PreviousHUDState;

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetHUDState(ETBSHUDState NewState);
	
	ATBSHUD(const FObjectInitializer& ObjectInitializer);
};
