// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TBS.h"
#include "TBSGameMode.h"
#include "TBSPlayerController.h"
#include "TBSCharacter.h"

ATBSGameMode::ATBSGameMode (const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer) {
	// use our custom PlayerController class
	PlayerControllerClass = ATBSPlayerController::StaticClass ();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass (TEXT ("/Game/TheBarberShop/Assets/Character_BP"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}