// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSPlayerController.h"
#include "TBSCharacter.h"
#include "TBSGameState.h"

#include "TBSSaveGame_SessionState.h"
#include "TBSSaveGame_Equipment.h"

ATBSGameState::ATBSGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	// @Notice: For some reason, this variable gets set, but when ::LoadGame() is called is pointing to null again...
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
bool ATBSGameState::SaveGame(int32 UserIndex) {
	CurrentSaveGame->ObtainedEquipmentNum = CurrentSaveGame->ObtainedEquipment.Num();
	CurrentSaveGame->SessionListNum = CurrentSaveGame->SessionList.Num();

	UTBSSaveGame_Equipment* EquipmentSaveState = Cast<UTBSSaveGame_Equipment>(UGameplayStatics::CreateSaveGameObject(UTBSSaveGame_Equipment::StaticClass()));
	// Save ObtainedEquipment
	for (int32 i = 0; i < CurrentSaveGame->ObtainedEquipmentNum; i++) {
		FString equipmentFileName = FString("Equipment");
		equipmentFileName.AppendInt(i);

		EquipmentSaveState->EquipmentID = CurrentSaveGame->ObtainedEquipment[i];
		if (!UGameplayStatics::SaveGameToSlot(EquipmentSaveState, equipmentFileName, UserIndex)) {
			return false;
		}
	}

	UTBSSaveGame_SessionState* EquipmentSessionState = Cast<UTBSSaveGame_SessionState>(UGameplayStatics::CreateSaveGameObject(UTBSSaveGame_SessionState::StaticClass()));
	// Save SessionList
	for (int32 i = 0; i < CurrentSaveGame->SessionListNum; i++) {
		FString sessionFileName = FString("Session");
		sessionFileName.AppendInt(i);

		EquipmentSessionState->Session = CurrentSaveGame->SessionList[i];
		if (!UGameplayStatics::SaveGameToSlot(EquipmentSessionState, sessionFileName, UserIndex)) {
			return false;
		}
	}

	UTBSSaveGame* TempSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::LoadGameFromSlot("Meta", UserIndex));
	// Save Meta
	// Current game info is automatically updated by methods that handle values that should be stored in a savefile
	// All we do here, is redirect the call and prevent accidental overwriting of savefiles
	if (!UGameplayStatics::SaveGameToSlot(CurrentSaveGame, "Meta", UserIndex)) {
		return false;
	}

	return true;
}
bool ATBSGameState::LoadGame(int32 UserIndex) {
	UTBSSaveGame* TempSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::LoadGameFromSlot("Meta", UserIndex));

	if (TempSaveGame != nullptr) {
		CurrentSaveGame = TempSaveGame;

		ATBSCharacter* PlayerPawn = ((ATBSCharacter*)(GetWorld()->GetFirstPlayerController()->GetPawn()));

		// Reset values if there were previous states
		PlayerPawn->CurrentCash = 0;
		PlayerPawn->CurrentLevel = 0;
		PlayerPawn->CurrentExperience = 0;

		PlayerPawn->ObtainedRazorSmall = CurrentSaveGame->ObtainedRazorSmall;
		PlayerPawn->ObtainedRazorBig = CurrentSaveGame->ObtainedRazorBig;

		// Apply values from savefile
		PlayerPawn->CurrentCash += CurrentSaveGame->MoneyAvailable;
		PlayerPawn->CurrentLevel = CurrentSaveGame->CurrentLevel;
		PlayerPawn->IncreaseEXP(CurrentSaveGame->ShaveperiencePoints);
		UTBSSaveGame_Equipment* EquipmentSaveState = Cast<UTBSSaveGame_Equipment>(UGameplayStatics::CreateSaveGameObject(UTBSSaveGame_Equipment::StaticClass()));
		// Load ObtainedEquipment
		for (int32 i = 0; i < CurrentSaveGame->ObtainedEquipmentNum; i++) {
			FString equipmentFileName = FString("Equipment");
			equipmentFileName.AppendInt(i);

			UTBSSaveGame_Equipment* equipmentSaveState = Cast<UTBSSaveGame_Equipment>(UGameplayStatics::LoadGameFromSlot(equipmentFileName, UserIndex));
			if (equipmentSaveState == nullptr) {
				UE_LOG(LogClass, Warning, TEXT("*** Tried to load equipment save %d/%d, which should exist, but does not! ***"), i, CurrentSaveGame->ObtainedEquipmentNum);
			}
			else {
				CurrentSaveGame->ObtainedEquipment.Add(equipmentSaveState->EquipmentID);
			}
		}

		UTBSSaveGame_SessionState* EquipmentSessionState = Cast<UTBSSaveGame_SessionState>(UGameplayStatics::CreateSaveGameObject(UTBSSaveGame_SessionState::StaticClass()));
		// Load SessionList
		for (int32 i = 0; i < CurrentSaveGame->SessionListNum; i++) {
			FString sessionFileName = FString("Session");
			sessionFileName.AppendInt(i);

			UTBSSaveGame_SessionState* SessionSaveState = Cast<UTBSSaveGame_SessionState>(UGameplayStatics::LoadGameFromSlot(sessionFileName, UserIndex));
			if (SessionSaveState == nullptr) {
				UE_LOG(LogClass, Warning, TEXT("*** Tried to load equipment save %d/%d, which should exist, but does not! ***"), i, CurrentSaveGame->ObtainedEquipmentNum);
			}
			else {
				CurrentSaveGame->SessionList.Add(SessionSaveState->Session);
			}
		}

		return true;
	}

	if (CurrentSaveGame == nullptr)
	{
		CurrentSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::CreateSaveGameObject(UTBSSaveGame::StaticClass()));
	}

	return TempSaveGame != nullptr;
}

float ATBSGameState::GetBestScore() {
	float BestScore = -1.0f;

	for (auto SaveGameIterator(CurrentSaveGame->SessionList.CreateIterator()); SaveGameIterator; SaveGameIterator++) {
		if (SaveGameIterator->BeardResult > BestScore || BestScore == -1.0f) {
			BestScore = SaveGameIterator->BeardResult;
		}
	}

	return BestScore;
}

float ATBSGameState::GetBestTime() {
	float BestTime = -1.0f;

	for (auto SaveGameIterator(CurrentSaveGame->SessionList.CreateIterator()); SaveGameIterator; SaveGameIterator++) {
		if (SaveGameIterator->TimeRequired < BestTime || BestTime == -1.0f) {
			BestTime = SaveGameIterator->TimeRequired;
		}
	}
	return BestTime;
}

bool ATBSGameState::WipeSaveFiles(int32 UserIndex) {
	CurrentSaveGame = Cast<UTBSSaveGame>(UGameplayStatics::LoadGameFromSlot("Meta", UserIndex));

	// Delete all sessions
	FString fileName;
	for (int i = 0; i < CurrentSaveGame->SessionListNum; i++) {
		fileName = FString("Session");
		fileName.AppendInt(i);
		UGameplayStatics::DeleteGameInSlot(fileName, UserIndex);
	}

	// Delete all equipment info
	for (int i = 0; i < CurrentSaveGame->ObtainedEquipmentNum; i++) {
		fileName = FString("Equipment");
		fileName.AppendInt(i);
		UGameplayStatics::DeleteGameInSlot(fileName, UserIndex);
	}

	// Delete additional files
	UGameplayStatics::DeleteGameInSlot("Meta", UserIndex);
	UGameplayStatics::DeleteGameInSlot("Settings", UserIndex);

	return true;
}

bool ATBSGameState::SaveFilesExist(int32 UserIndex) {
	return UGameplayStatics::DoesSaveGameExist("Meta", UserIndex);
}

#pragma endregion
