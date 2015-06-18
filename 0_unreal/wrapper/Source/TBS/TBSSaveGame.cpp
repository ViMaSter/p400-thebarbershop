// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSSaveGame.h"

UTBSSaveGame::UTBSSaveGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SaveSlotName = TEXT("EMPTY");
	UserIndex = 0;
}

#pragma region SessionState
int32 UTBSSaveGame::AddSessionState(FTBSSessionState SessionState) {
	return SessionList.Add(SessionState);
}

int32 UTBSSaveGame::RemoveSessionState(FTBSSessionState SessionState) {
	return SessionList.Remove(SessionState);
}

TArray<FTBSSessionState> UTBSSaveGame::GetSessionState(FTBSSessionState SessionState) {
	return TArray<FTBSSessionState>();
}

bool UTBSSaveGame::ClearSessionStates() {
	SessionList.Empty();
	return SessionList.Num() == 0;
}
#pragma endregion