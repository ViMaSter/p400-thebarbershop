// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSHUD.h"

ATBSHUD::ATBSHUD (const class FObjectInitializer& PCIP)
	: Super (PCIP) {
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
}

