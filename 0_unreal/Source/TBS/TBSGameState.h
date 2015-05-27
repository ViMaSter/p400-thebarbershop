// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSSessionState.h"
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
	 bool IsPaused = true;

public:
#pragma region GameState
	UFUNCTION(BlueprintCallable, Category = "Game State") void SetIsIngame(bool NewState);
	UFUNCTION(BlueprintCallable, Category = "Game State") bool GetIsIngame();

	UFUNCTION(BlueprintCallable, Category = "Game State") void SetIsEditorMode(bool NewState);
	UFUNCTION(BlueprintCallable, Category = "Game State") bool GetIsEditorMode();

	UFUNCTION(BlueprintCallable, Category = "Game State") void SetIsPaused(bool NewState);
	UFUNCTION(BlueprintCallable, Category = "Game State") bool GetIsPaused();
#pragma endregion

#pragma region SessionState
	// @TODO: Perhabs rethink exposing this variable directly.
	// Removing and adding elements seems unnecessarily complicated at this point in time
	int32 AddSessionState(FTBSSessionState SessionState);
	int32 RemoveSessionState(FTBSSessionState SessionState);
	TArray<FTBSSessionState> GetSessionState(FTBSSessionState SessionState);
	bool EmptySessionState();

private:
	TArray<FTBSSessionState> SessionList;

#pragma endregion
};
