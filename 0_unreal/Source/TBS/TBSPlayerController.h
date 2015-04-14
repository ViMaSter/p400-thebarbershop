// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
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
	void RotateRazorTop(float Value);
	void RotateRazorRight(float Value);
	void SwitchToNextTool();
	void SwitchToPrevTool();
	void UpdateRazorPosition();
	void OnSetShavedPressed();
	void OnSetShavedReleased();
	void OnSetRotationPressed();
	void OnSetRotationReleased();

};


