// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGameState.h"

void ATBSGameState::SetIsIngame(bool NewState) {
	IsIngame = NewState;
}

bool ATBSGameState::GetIsIngame() {
	return IsIngame;
}

void ATBSGameState::SetIsEditorMode(bool NewState) {
	IsEditorMode = NewState;
}

bool ATBSGameState::GetIsEditorMode() {
	return IsEditorMode;
}

void ATBSGameState::SetIsPaused(bool NewState) {
	IsPaused = NewState;
}

bool ATBSGameState::GetIsPaused() {
	return IsPaused;
}