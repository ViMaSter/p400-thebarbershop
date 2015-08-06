// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Engine/DataTable.h"
#include "TBSEquipment.generated.h"

UENUM(BlueprintType)
namespace ETBSRazor {
	enum Type {
		TBSClipper = 0 	UMETA(DisplayName = "Clipper"),
		TBSRazorSmall = 1		UMETA(DisplayName = "RazorSmall"),
		TBSRazorBig = 2		UMETA(DisplayName = "RazorBig"),
	};
}

UENUM (BlueprintType)
enum class ETBSEquipmentType : uint8 {
	NONE			= 0		UMETA (DisplayName = "NONE"),
	Towel			= 1		UMETA (DisplayName = "Towel"),
	Clipper			= 2		UMETA (DisplayName = "Clipper"),
	RazorSmall		= 3		UMETA(DisplayName = "RazorSmall"),
	RazorLarge		= 4		UMETA(DisplayName = "RazorLarge")
};

USTRUCT(BlueprintType)
struct FTBSEquipmentData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FTBSEquipmentData()
		: EquipmentID(0)
		, Type(ETBSEquipmentType::NONE)
		, Name("Default")
		, Icon(nullptr)
		, Cost(0) {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		int32 EquipmentID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		TEnumAsByte<ETBSEquipmentType> Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		int32 Cost;
};


USTRUCT(BlueprintType)
struct FTBSItem {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		TEnumAsByte<ETBSEquipmentType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		int32 EquipmentID;

};