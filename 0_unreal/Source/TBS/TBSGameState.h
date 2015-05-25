// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSGameState.generated.h"

/**
 *
 * Using this Class for session storage and general game properties!
 * @Vincent think we should use this class for saving and loading
 * and for showing the last result in comparison
 * 
 */
UCLASS()
class TBS_API ATBSGameState : public AGameState
{
	GENERATED_BODY()

private:
	 bool IsIngame = false;
	 bool IsEditorMode = false;
	 bool IsPaused = false;

public:
	void SetIsIngame(bool NewState);
	bool GetIsIngame();

	void SetIsEditorMode(bool NewState);
	bool GetIsEditorMode();

	void SetIsPaused(bool NewState);
	bool GetIsPaused();
};
