// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TBSSessionState.h"
#include "TBSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TBS_API UTBSSaveGame : public USaveGame
{
	GENERATED_BODY()
		
	UTBSSaveGame(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Core") FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Core") uint32 UserIndex;
											
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") float MoneyAvailable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") float ShaveperiencePoints;

#pragma region SessionState
	int32 AddSessionState(FTBSSessionState SessionState);
	int32 RemoveSessionState(FTBSSessionState SessionState);
	TArray<FTBSSessionState> GetSessionState(FTBSSessionState SessionState);
	bool ClearSessionStates();

private:
	TArray<FTBSSessionState> SessionList;
#pragma endregion
	
};
