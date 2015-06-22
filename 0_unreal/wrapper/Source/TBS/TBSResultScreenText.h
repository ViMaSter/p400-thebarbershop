// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Engine/DataTable.h"

#include "TBSResultScreenText.generated.h"

UENUM(BlueprintType)
enum class ETBSResultScreenTextType : uint8 {
	NONE							= 0		UMETA(DisplayName = "NONE"),
	GeneralHint						= 1		UMETA(DisplayName = "General hint"),
	OtherToolsHint					= 2		UMETA(DisplayName = "Other tools hint"),
	BetterPercentageThanLastTry		= 3		UMETA(DisplayName = "Better percentage than last try"),
	BetterTimeThanLastTry			= 4		UMETA(DisplayName = "Better time than last try"),
	WorsePercentageThanLastTry		= 5		UMETA(DisplayName = "Worse percentage than last try"),
	WorseTimeThanLastTry			= 6		UMETA(DisplayName = "Worse time than last try"),
};

USTRUCT(BlueprintType)
struct FTBSResultScreenText : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	FTBSResultScreenText()
		: Type(ETBSResultScreenTextType::NONE)
		, Content("EMPTY") {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		ETBSResultScreenTextType Type;

	/*
	Variables to use in content:
	{oldbestpercentage} = best result before this
	{percentage} = result
	{deltapercentage} = result - best result before this
	{absolutedeltapercentage} = abs(result - best result before this)

	{oldbesttime} = best result before this
	{time} = time
	{deltatime} = time - best time before this
	{absolutedeltatime} = abs(time - best time before this)

	{wordedtime} = (x minutes and) y seconds
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FName Content;

	/*
		Likelihood of a message being thrown.
		Setting this to -1 on a message, guarantees that this message is thrown, if the selected condition is met.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		float Weight;
};