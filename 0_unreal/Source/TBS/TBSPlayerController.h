// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "TBSCustomer.h"
#include "TBSCharacter.h"
#include "Engine/LocalPlayer.h"
#include "TBSPlayerController.generated.h"

UCLASS()
class ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBSPlayerController(const FObjectInitializer& ObjectInitializer);


protected:

	bool ShaveActive;
	bool RotationActive;
	FVector2D StoredMousePosition;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void RotateTop(float Value);
	void RotateRight(float Value);
	void RotateToolTop(float Value);
	void RotateToolRight(float Value);
	void SwitchToNextTool();
	void SwitchToPrevTool();
	void UpdateRazorPosition();
	void OnSetShavedPressed();
	void OnSetShavedReleased();
	void OnSetRotationPressed();
	void OnSetRotationReleased();

	// Pitch Hack
	void SpawnNextCustomer();

	// Beared Data Control
	UFUNCTION(exec)	void ClearAllBeardData();
	UFUNCTION(exec)	void ClearBeardID(FName BeardName);
	UFUNCTION(exec) void SaveBeardID(FName BeardName);
	UFUNCTION(exec) void AddBeardID(FName BeardName);
	UFUNCTION(exec) void ReplaceBeardID(FName BeardName);
	void SetCurrentBeardDataToCSV(FBeardComparisonData* CurrentData);
};


