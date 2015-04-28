// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSRazor.h"


ATBSRazor::ATBSRazor () {
	PrimaryActorTick.bCanEverTick = true;

	ToolInactiveHight = 10;
	IsActive = false;
	ToolType = ETBSRazor::TBSRazorBig;
}

void ATBSRazor::Tick (float DeltaTime) {
	Super::Tick (DeltaTime);
}

void ATBSRazor::SwitchRazorTypeTo (TEnumAsByte<ETBSRazor::Type> NewType) {
	if (NewType == ToolType) {
		return;
	}

	ToolType = NewType;

	// Notify BP
	SwitchedTool (NewType.GetValue ());
	UE_LOG (LogClass, Log, TEXT ("*** Switched to %d ***"), (int) NewType);
}