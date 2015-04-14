// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSRazor.h"


ATBSRazor::ATBSRazor()
{
	PrimaryActorTick.bCanEverTick = true;

	ToolInactiveHight = 10;
	IsActive = false;
	ToolType = ETBSRazor::TBSRazorBig;
}

void ATBSRazor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATBSRazor::SwitchRazorTypeTo(TEnumAsByte<ETBSRazor::Type> Type){
	if (Type == ToolType)	return;
	switch (Type)
	{
	case ETBSRazor::TBSHand:
		ToolType = ETBSRazor::TBSHand;
		SwitchedToHand();
		UE_LOG(LogClass, Log, TEXT("*** Switched to HandTool ***"));
		break;
	case ETBSRazor::TBSClipper:
		ToolType = ETBSRazor::TBSClipper;
		SwitchedToClipper();
		UE_LOG(LogClass, Log, TEXT("*** Switched to ClipperTool ***"));
		break;
	case ETBSRazor::TBSRazorSmall:
		ToolType = ETBSRazor::TBSRazorSmall;
		SwitchedToRazorSmall();
		UE_LOG(LogClass, Log, TEXT("*** Switched to SmallRazorTool ***"));
		break;
	case ETBSRazor::TBSRazorBig:
		ToolType = ETBSRazor::TBSRazorBig;
		SwitchedToRazorBig();
		UE_LOG(LogClass, Log, TEXT("*** Switched to BigRazorTool ***"));
		break;
	}
}