// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGameState.h"

#pragma region GameState
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

#pragma endregion

#pragma region SessionState

int32 ATBSGameState::AddSessionState(FTBSSessionState SessionState) {
	return SessionList.Add(SessionState);
}

int32 ATBSGameState::RemoveSessionState(FTBSSessionState SessionState) {
	return SessionList.Remove(SessionState);
}

TArray<FTBSSessionState> ATBSGameState::GetSessionState(FTBSSessionState SessionState) {

	return TArray<FTBSSessionState>();
}

bool ATBSGameState::EmptySessionState() {
	SessionList.Empty();
	return SessionList.Num() == 0;
}

#pragma endregion
