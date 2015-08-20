// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "TBSSessionState.h"
#include "TBSSaveGame.generated.h"

UCLASS()
class TBS_API UTBSSaveGame : public USaveGame
{
	GENERATED_BODY()
		
	UTBSSaveGame(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Core") FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Core") int32 UserIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") bool UsedOtherTools;
	UFUNCTION(BlueprintCallable, Category = "TBS") bool ShouldTutorialTextBeShown();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") float MoneyAvailable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") float CashEarned;

	// We only store the absolute experience points - we can calculate the resulting level from that
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") int32 CurrentLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TBS") float ShaveperiencePoints;

	// Lenghts
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lengths") int32 ObtainedEquipmentNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lengths") int32 SessionListNum;

	TArray<int32> ObtainedEquipment;
#pragma region SessionState
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Session") TArray<FTBSSessionState> SessionList;

	int32 AddSessionState(FTBSSessionState SessionState);
	int32 RemoveSessionState(FTBSSessionState SessionState);
	bool ClearSessionStates();
#pragma endregion
	
};
