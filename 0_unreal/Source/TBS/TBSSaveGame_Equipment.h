// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TBSSaveGame_Equipment.generated.h"

UCLASS()
class TBS_API UTBSSaveGame_Equipment : public USaveGame {
	GENERATED_BODY()

	UTBSSaveGame_Equipment(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") int32 EquipmentID;
};