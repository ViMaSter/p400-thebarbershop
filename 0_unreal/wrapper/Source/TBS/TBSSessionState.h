// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "TBSSessionState.generated.h"

USTRUCT(BlueprintType)
struct FTBSSessionState {
	GENERATED_USTRUCT_BODY()

public:
	FTBSSessionState()
		: SessionID(0)
		, BeardName("NotInit")
		, BeardResult(0.0f)
		, TimeRequired(0.0f) {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	int32 SessionID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	FName BeardName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	float BeardResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	float TimeRequired;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	int32 TimesHurt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Information")	bool SatisfiedCustomer;

	bool operator==(FTBSSessionState const& Other) const {
		return SessionID == Other.SessionID;
	}
};