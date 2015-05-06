// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSCustomer.h"
#include "TBSCharacter.h"


// Sets default values
ATBSCustomer::ATBSCustomer (const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	/* FClassfinder does not find the Blueprint for some reason. So it resets for some reason the Bearclass with Null.
	   This is a short fix, so the autobuilt will work properly.
	   static ConstructorHelpers::FClassFinder<AActor> BeardBP(TEXT("/Game/TheBarberShop/Assets/Characters/BeardStyles/BeardStyle_00_BP"));
	   if (BeardBP.Class != NULL){
	   BeardClass = BeardBP.Class;
	   }*/

}
// Called when the game starts or when spawned
void ATBSCustomer::BeginPlay () {
	Super::BeginPlay ();
	if (BeardClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		Beard = GetWorld()->SpawnActor<AActor>(BeardClass, FVector(0, 0, 340), FRotator::ZeroRotator, SpawnParams);
	}
	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
}

void ATBSCustomer::CreateNewCustomer (int32 CharacterLevel) {
	// Reset the beard to Trimmed 0 and visibile.
	TArray<UActorComponent*> Components;
	Components = Beard->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (int32 i = 0; i < Components.Num(); i++) {
		UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Components[i];
		Mesh->SetVisibility(true);
		Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
		((ATBSCharacter*)GetOwner())->Tool->Trimmed(0, Components[i]);
	}

	CreatedCustomer();
	UE_LOG(LogClass, Log, TEXT("*** Customer Customer created ***"))
}

