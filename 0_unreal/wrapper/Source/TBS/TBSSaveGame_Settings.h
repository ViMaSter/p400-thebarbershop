// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TBSSaveGame_Settings.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API UTBSSaveGame_Settings : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS") int32 MusicVolume = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS") int32 SFXVolume =	100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS") int32 ResolutionX = 1280;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS") int32 ResolutionY = 720;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS") bool IsFullscreen = false;
	
};
