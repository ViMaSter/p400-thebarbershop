// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "TBSSaveGame.h"
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

	ATBSGameState(const FObjectInitializer& ObjectInitializer);
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
#pragma region SaveState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save State") UTBSSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable, Category = "Save State") bool SaveGame(int32 UserIndex);
	UFUNCTION(BlueprintCallable, Category = "Save State") bool LoadGame(int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category = "Save State") float GetBestScore();
	UFUNCTION(BlueprintCallable, Category = "Save State") float GetBestTime();
#pragma endregion
};
