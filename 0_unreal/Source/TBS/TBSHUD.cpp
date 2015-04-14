// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSHUD.h"

ATBSHUD::ATBSHUD(const class FObjectInitializer& PCIP)
: Super(PCIP)
//, CurrentHUDState(EHUDState::None)
//, PreviousHUDState(EHUDState::None)
{
	//SetHUDState(EHUDState::Intro);
}

void ATBSHUD::SetHUDState(ETBSHUDState NewState)
{
	PreviousHUDState = CurrentHUDState;
	CurrentHUDState = NewState;
}

