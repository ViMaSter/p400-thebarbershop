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

	// Vincent: Needed move this from protected to public, since this methods will be called from the HUD
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void ClearBeardID(FString BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void SaveBeardID(FString BeardName);
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void LoadBeardID(FString BeardName);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Ignore")
	bool InputIgnore;

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

	// Beard Control
	UFUNCTION(BlueprintCallable, exec, Category = "Beard Control") void SpawnNextCustomer();
	void LoadBeardDataToCurrentCustomer(UDataTable* Datatable);
	void SetCurrentBeardDataToCSV(UDataTable* DataTable);

};


