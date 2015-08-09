// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "StateComponent.h"
#include "TBSHUDState.h"
#include "TBSHUD.generated.h"

/**
 *
 */
UCLASS()
class TBS_API ATBSHUD : public AHUD {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		UStateComponent* StateComponent;

	ATBSHUD(const FObjectInitializer& ObjectInitializer);

};