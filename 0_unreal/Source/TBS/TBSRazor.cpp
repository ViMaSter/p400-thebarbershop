// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSRazor.h"


ATBSRazor::ATBSRazor()
{
	PrimaryActorTick.bCanEverTick = true;

	RazorHightInactive = 10;
	IsActive = false;
	RazorType = ETBSRazor::TBSRazorBig;
}

void ATBSRazor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATBSRazor::SwitchRazorTypeTo(TEnumAsByte<ETBSRazor::Type> Type){
	if (Type == RazorType)	return;
	switch (Type)
	{
	case ETBSRazor::TBSHand:
		RazorType = ETBSRazor::TBSHand;
		SwitchedToHand();
		UE_LOG(LogClass, Log, TEXT("*** Switched to HandTool ***"));
		break;
	case ETBSRazor::TBSClipper:
		RazorType = ETBSRazor::TBSClipper;
		SwitchedToClipper();
		UE_LOG(LogClass, Log, TEXT("*** Switched to ClipperTool ***"));
		break;
	case ETBSRazor::TBSRazorSmall:
		RazorType = ETBSRazor::TBSRazorSmall;
		SwitchedToRazorSmall();
		UE_LOG(LogClass, Log, TEXT("*** Switched to SmallRazorTool ***"));
		break;
	case ETBSRazor::TBSRazorBig:
		RazorType = ETBSRazor::TBSRazorBig;
		SwitchedToRazorBig();
		UE_LOG(LogClass, Log, TEXT("*** Switched to BigRazorTool ***"));
		break;
	}
}