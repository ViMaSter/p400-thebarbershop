// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSRazor.generated.h"

UENUM (BlueprintType)
namespace ETBSRazor {
	enum Type {
		TBSHand			= 0		UMETA (DisplayName = "Hand"),
		TBSClipper		= 1 	UMETA (DisplayName = "Clipper"),
		TBSRazorSmall	= 2		UMETA (DisplayName = "RazorSmall"),
		TBSRazorBig		= 3		UMETA (DisplayName = "RazorBig"),
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

	virtual void Tick (float DeltaSeconds) override;

};
