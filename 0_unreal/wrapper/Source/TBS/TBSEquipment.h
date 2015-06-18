// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Engine/DataTable.h"

#include "TBSEquipment.generated.h"

UENUM (BlueprintType)
enum class ETBSEquipmentType : uint8 {
	NONE			= 0		UMETA (DisplayName = "NONE"),
	Towel			= 1		UMETA (DisplayName = "Towel"),
	Clipper			= 2		UMETA (DisplayName = "Clipper"),
	Razor			= 3		UMETA (DisplayName = "Razor")
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
