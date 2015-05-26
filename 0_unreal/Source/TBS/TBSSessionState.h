// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "TBSSessionState.generated.h"

USTRUCT(BlueprintType)
struct FTBSSessionState {
	GENERATED_USTRUCT_BODY()

public:
	FTBSSessionState()
		: SessionID(0)
		, BeardID(0)
		, BeardResult(0.0f)
		, TimeRequired(0.0f) {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	uint32 SessionID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	int32 BeardID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	float BeardResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	float TimeRequired;
};