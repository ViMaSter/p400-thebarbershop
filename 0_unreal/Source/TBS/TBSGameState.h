// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSGameState.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API ATBSGameState : public AGameState
{
	GENERATED_BODY()

private:
	bool IsEditorModeActive = false;
	bool IsPaused = false;

public:
	void SetEditorModeActive(bool IsEditorMode);
	bool GetEditorModeActive();

	void SetPaused(bool IsPaused);
	bool GetPaused();
	void TogglePause();
};
