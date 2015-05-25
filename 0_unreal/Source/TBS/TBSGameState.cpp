// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGameState.h"

void ATBSGameState::TogglePause() {
	IsPaused = !IsPaused;
}

void ATBSGameState::SetEditorModeActive(bool IsEditorMode) {
	IsEditorModeActive = IsEditorMode;
}

bool ATBSGameState::GetEditorModeActive()
{
	return IsEditorModeActive;
}

void ATBSGameState::SetPaused(bool IsPaused)
{
	this->IsPaused = IsPaused;
}

bool ATBSGameState::GetPaused()
{
	return IsPaused;
}
