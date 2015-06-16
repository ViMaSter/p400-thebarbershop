// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TBSSessionState.h"
#include "TBSSaveGame_SessionState.generated.h"

UCLASS()
class TBS_API UTBSSaveGame_SessionState : public USaveGame {
	GENERATED_BODY()

	UTBSSaveGame_SessionState(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") FTBSSessionState Session;
};