// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TBSEquipment.h"
#include "TBSRazor.generated.h"


UCLASS()
class TBS_API ATBSRazor : public AActor {
	GENERATED_BODY ()
public:
	ATBSRazor ();

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "RazorStatus")
	TEnumAsByte<ETBSRazor::Type> ToolType;


	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "RazorStatus")
	int32 ToolInactiveHight;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "RazorStatus")
	float TrimmAmount;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "RazorStatus")
	bool IsActive;


	UFUNCTION (BlueprintImplementableEvent, Category = "Razor")
	void SwitchedTool (ETBSRazor::Type NewToolType);

	UFUNCTION (BlueprintImplementableEvent, Category = "Beard")
	void TrimmHair (int32 index);

	UFUNCTION(BlueprintImplementableEvent, Category = "Beard")
	void ShaveHair(int32 index);

	UFUNCTION(BlueprintImplementableEvent, Category = "Beard")
	void ResetHair(int32 index);

	UFUNCTION(BlueprintImplementableEvent, Category = "Beard")
	void ResetHairs();

	UFUNCTION(BlueprintImplementableEvent, Category = "Beard")
	void ResetEditorHairs();

	void SwitchRazorTypeTo (TEnumAsByte<ETBSRazor::Type> NewType);

	virtual void Tick(float DeltaSeconds) override;	
	
	// Current Customer ISMArrays

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	TArray<UInstancedStaticMeshComponent*> ISMNormalTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	TArray<UInstancedStaticMeshComponent*> ISMNormalFillerTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	TArray<UInstancedStaticMeshComponent*> ISMTrimmedTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	TArray<UInstancedStaticMeshComponent*> ISMTrimmedFillerTotal;

	// Capture Customer ISMArrays

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMCaptureNormalTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMCaptureNormalFillerTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMCaptureTrimmedTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMCaptureTrimmedFillerTotal;
	
	// TMP/ Next Customer ISMArrays (for Reset)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMTmpNormalTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMTmpNormalFillerTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMTmpTrimmedTotal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
		TArray<UInstancedStaticMeshComponent*> ISMTmpTrimmedFillerTotal;

};
