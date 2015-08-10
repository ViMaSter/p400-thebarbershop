// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSSaveGame.h"

UTBSSaveGame::UTBSSaveGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SaveSlotName = TEXT("EMPTY");
	UserIndex = 0;
	CurrentLevel = 1;
}

#pragma region SessionState
int32 UTBSSaveGame::AddSessionState(FTBSSessionState SessionState) {
	return SessionList.Add(SessionState);
}

int32 UTBSSaveGame::RemoveSessionState(FTBSSessionState SessionState) {
	return SessionList.Remove(SessionState);
}

bool UTBSSaveGame::ClearSessionStates() {
	SessionList.Empty();
	return SessionList.Num() == 0;
}
bool UTBSSaveGame::ShouldTutorialTextBeShown() {
	return !(SessionList.Num() == 0) && !UsedOtherTools;
}
#pragma endregion