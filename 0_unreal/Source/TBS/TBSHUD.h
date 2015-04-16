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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State") TEnumAsByte<ETBSHUDState> CurrentHUDState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State") TEnumAsByte<ETBSHUDState> PreviousHUDState;

	UFUNCTION(BlueprintCallable, Category = "State")
		void SetHUDState(ETBSHUDState NewState);

	UFUNCTION(BlueprintCallable, Category = "State")
		void SetHUDStateUsingByte(TEnumAsByte<ETBSHUDState> NewState);

	UFUNCTION(BlueprintCallable, Category = "State")
		bool IsHUDState(TEnumAsByte<ETBSHUDState> NewState);
	UFUNCTION(BlueprintCallable, Category = "State")
		void AddHUDState(TEnumAsByte<ETBSHUDState> NewState);
	UFUNCTION(BlueprintCallable, Category = "State")
		void RemoveHUDState(TEnumAsByte<ETBSHUDState> NewState);
	UFUNCTION(BlueprintCallable, Category = "State")
		void ToggleHUDState(TEnumAsByte<ETBSHUDState> NewState);

	
	ATBSHUD(const FObjectInitializer& ObjectInitializer);
};
