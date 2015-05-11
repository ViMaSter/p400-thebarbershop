// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "TBSHUDState.generated.h"

UENUM (BlueprintType)
enum class ETBSHUDState : uint8 {
	None			= 0		UMETA (DisplayName = "None"),
	Intro			= 1		UMETA (DisplayName = "Intro"),
	StartScreen		= 2		UMETA (DisplayName = "Start Screen"),
	Options			= 3		UMETA (DisplayName = "Options"),
	Options_Audio	= 4		UMETA (DisplayName = "Options Audio"),
	Options_Video	= 5		UMETA (DisplayName = "Options Video"),
	Ingame			= 6		UMETA (DisplayName = "Ingame"),
	Editor			= 7		UMETA(DisplayName = "Editor"),
	Results			= 8		UMETA(DisplayName = "Results")
};


UENUM (BlueprintType)
enum class ETBSHUDAnimationDirection : uint8 {
	None			= 0		UMETA (DisplayName = "None"),
	Up				= 1		UMETA (DisplayName = "Up"),
	Down			= 2		UMETA (DisplayName = "Down"),
	Left			= 4		UMETA (DisplayName = "Left"),
	Right			= 8		UMETA (DisplayName = "Right")
};

ENUM_CLASS_FLAGS (ETBSHUDAnimationDirection)