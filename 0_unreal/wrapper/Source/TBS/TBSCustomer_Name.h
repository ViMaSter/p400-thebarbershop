// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "TBSCustomer_Name.generated.h"

USTRUCT(BlueprintType)
struct FCustomerName : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FCustomerName()
		: FirstName("")
		, SurName("") {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
		FName FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
		FName SurName;
};
