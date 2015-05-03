// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSHUD.h"

ATBSHUD::ATBSHUD (const class FObjectInitializer& PCIP)
	: Super (PCIP)
	, CurrentHUDState (ETBSHUDState::None)
	, PreviousHUDState (ETBSHUDState::None) {
	SetHUDState (ETBSHUDState::Intro);
}

void ATBSHUD::SetHUDState (ETBSHUDState NewState) {
	PreviousHUDState = CurrentHUDState;
	CurrentHUDState = NewState;
}

void ATBSHUD::SetHUDStateUsingByte (TEnumAsByte<ETBSHUDState> NewState) {
	PreviousHUDState = CurrentHUDState;
	CurrentHUDState = NewState;
}

bool ATBSHUD::IsHUDState (TEnumAsByte<ETBSHUDState> NewState) {
	return ((int) CurrentHUDState.GetValue () & (int) NewState.GetValue ()) == (int) NewState.GetValue ();
}

void ATBSHUD::AddHUDState (TEnumAsByte<ETBSHUDState> NewState) {
	return SetHUDState ((TEnumAsByte<ETBSHUDState>)((int) CurrentHUDState.GetValue () | (int) NewState.GetValue ()));
}

void ATBSHUD::RemoveHUDState (TEnumAsByte<ETBSHUDState> NewState) {
	return SetHUDState ((TEnumAsByte<ETBSHUDState>)((int) CurrentHUDState.GetValue () & ~(int) NewState.GetValue ()));
}

void ATBSHUD::ToggleHUDState (TEnumAsByte<ETBSHUDState> NewState) {
	if (IsHUDState (NewState)) {
		return RemoveHUDState (NewState);
	}
	else {
		return AddHUDState (NewState);
	}
}
