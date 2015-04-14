// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSCustomer.h"


// Sets default values
ATBSCustomer::ATBSCustomer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FClassFinder<AActor> BearedBP(TEXT("/Game/TheBarberShop/Assets/Beard_02_BP"));
	//BearedClass = BearedBP.Class;
}

// Called when the game starts or when spawned
void ATBSCustomer::BeginPlay(){
	Super::BeginPlay();
	CreateNewCustomer();
}

void ATBSCustomer::CreateNewCustomer(){
	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
}

