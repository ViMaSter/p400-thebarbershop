// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSGameState.h"

ATBSGameState::ATBSGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	CurrentSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::CreateSaveGameObject(UTBSSaveGame::StaticClass()));
}

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

#pragma region SaveState
bool ATBSGameState::SaveGame(FString SlotName, int32 UserIndex, bool OverwriteIfExists = false) {
	UTBSSaveGame* TempSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (OverwriteIfExists || TempSaveGame == nullptr) {
		// Current game info is automatically updated by methods that handle values that should be stored in a savefile
		// All we do here, is redirect the call and prevent accidental overwriting of savefiles
		return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, UserIndex);
	}
	else {
		return false;
	}
}
bool ATBSGameState::LoadGame(FString SlotName, int32 UserIndex) {
	UTBSSaveGame* TempSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if (TempSaveGame != nullptr) {
		CurrentSaveGame = TempSaveGame;
	}

	// TODO: Redirect all changes that happen to the correct objects (update Cash in TBSPlayerController, etc.)

	return TempSaveGame != nullptr;

}
#pragma endregion
