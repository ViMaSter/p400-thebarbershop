// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Engine/DataTable.h"
#include "TBSProgression.generated.h"




USTRUCT(BlueprintType)
struct FLevelUpData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FLevelUpData()
		: Level(0)
		, XPtoLvl(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		int32 XPtoLvl;
};

USTRUCT(BlueprintType)
struct FTimeBonusData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FTimeBonusData()
		: TimeMin(0)
		, TimeMax(0)
		, BonusCash(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonusCash")
		int32 TimeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonusCash")
		int32 TimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BonusCash")
		int32 BonusCash;
};
