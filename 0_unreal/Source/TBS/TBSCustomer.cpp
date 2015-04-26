// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSCustomer.h"


// Sets default values
ATBSCustomer::ATBSCustomer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor> BeardBP(TEXT("/Game/TheBarberShop/Assets/Characters/BeardStyles/BeardStyle_00_BP"));
	BeardClass = BeardBP.Class;
	
}
// Called when the game starts or when spawned
void ATBSCustomer::BeginPlay(){
	Super::BeginPlay();

	CreateNewCustomer();
}

void ATBSCustomer::CreateNewCustomer(){
	if (Beard) {
		Beard->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Instigator;
	SpawnParams.Owner = this;
	Beard = GetWorld()->SpawnActor<AActor>(BeardClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	UStaticMeshComponent* Mesh = (UStaticMeshComponent*)GetComponentByClass(UStaticMeshComponent::StaticClass());
	FTransform Transf = Mesh->GetComponentTransform();

	if (Beard) {
		Beard->SetActorTransform(Transf);
	}
	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
}

