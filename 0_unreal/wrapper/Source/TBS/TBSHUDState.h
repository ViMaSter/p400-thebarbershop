// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "TBSHUDState.generated.h"

UENUM (BlueprintType)
enum class ETBSHUDState : uint8 {
	None			= 0		UMETA (DisplayName = "None"),
	Intro			= 1		UMETA (DisplayName = "Intro"),
	PauseMenu		= 2		UMETA (DisplayName = "Pause Menu"),
	Options			= 3		UMETA (DisplayName = "Options"),
	Options_Audio	= 4		UMETA (DisplayName = "Options Audio"),
	Options_Video	= 5		UMETA (DisplayName = "Options Video"),
	Ingame			= 6		UMETA (DisplayName = "Ingame"),
	Editor			= 7		UMETA (DisplayName = "Editor"),
	Results			= 8		UMETA (DisplayName = "Results"),
	Shop			= 9		UMETA (DisplayName = "Shop"),
	BeardOverview	= 10	UMETA (DisplayName = "Beard Overview"),
	MainMenu		= 11	UMETA (DisplayName = "Main Menu"),
	Radio			= 12	UMETA (DisplayName = "Radio"),
	EditorDialogue	= 13	UMETA (DisplayName = "Editor Dialogue"),
	MainMenuConfirm = 14	UMETA (DisplayName = "Main Menu Confirm")
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