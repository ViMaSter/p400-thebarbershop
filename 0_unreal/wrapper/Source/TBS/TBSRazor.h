// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TBSRazor.generated.h"

UENUM (BlueprintType)
namespace ETBSRazor {
	enum Type {
		TBSClipper		= 0 	UMETA (DisplayName = "Clipper"),
		TBSRazorSmall	= 1		UMETA (DisplayName = "RazorSmall"),
		TBSRazorBig		= 2		UMETA (DisplayName = "RazorBig"),
	};
}

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
	void Trimmed (float TrimmValue, UActorComponent* Mesh);

	void SwitchRazorTypeTo (TEnumAsByte<ETBSRazor::Type> NewType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	UInstancedStaticMeshComponent* TrimmedBeardInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	UInstancedStaticMeshComponent* InstancedSMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hairs Cut")
	TArray<UActorComponent*> CuttedHairs;
};
