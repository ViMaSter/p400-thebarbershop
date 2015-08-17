// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Engine/DataTable.h"
#include "TBSBeard.generated.h"


USTRUCT(BlueprintType)
struct FBeardNameLevelData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		FName BeardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		int32 BeardLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		int32 UniqueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		UTexture2D* ComparisionScreenshot;

};

USTRUCT(BlueprintType)
struct FBeardComparisonData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	FBeardComparisonData()
		: HairState(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairInfo")
		int32 HairState;
};

USTRUCT(BlueprintType)
struct FBeardCollectionData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	FBeardCollectionData()
		: BeardName("")
		, BeardSlotName("")
		, BeardLevel(0)
		, UniqueIdentifier(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		FName BeardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		FName BeardSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		int32 BeardLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		int32 UniqueIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardCollection")
		UTexture2D* ComparisionScreenshot;
};

USTRUCT(BlueprintType)
struct FBeardMeshData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	FBeardMeshData()
		: TranslateX(0)
		, TranslateY(0)
		, TranslateZ(0)
		, Roll(0)
		, Pitch(0)
		, Yaw(0)
		, ScaleX(1)
		, ScaleY(1)
		, ScaleZ(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Translation")	float TranslateZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")		float Roll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")		float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")		float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale")	float ScaleX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale")	float ScaleY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale")	float ScaleZ;
};

USTRUCT(BlueprintType)
struct FBeardPoolData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	FBeardPoolData()
		: PlayerLevel(0)
		, BeardMinNumber(1)
		, BeardMaxNumber(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardPool")	int32 PlayerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardPool")	int32 BeardMinNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BeardPool")	int32 BeardMaxNumber;
};