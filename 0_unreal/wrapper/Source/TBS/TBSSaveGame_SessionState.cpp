// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSSaveGame_SessionState.h"

UTBSSaveGame_SessionState::UTBSSaveGame_SessionState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	Session = FTBSSessionState();
}