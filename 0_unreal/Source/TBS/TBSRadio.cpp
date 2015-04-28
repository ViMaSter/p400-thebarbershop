// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSRadio.h"


// Sets default values
ATBSRadio::ATBSRadio () {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create a camera boom...
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo (RootComponent);

	Music0 = CreateDefaultSubobject<UAudioComponent>(TEXT("Music0"));
	Music0->AttachTo (Mesh);
	Music0->bOverrideAttenuation = true;
	Music0->bAllowSpatialization = true;

	Music0->AttenuationOverrides.bAttenuate = true;
	Music0->AttenuationOverrides.bSpatialize = true;
	Music0->AttenuationOverrides.AttenuationShape = EAttenuationShape::Cone;
	Music0->AttenuationOverrides.AttenuationShapeExtents.X = 510;
	Music0->AttenuationOverrides.AttenuationShapeExtents.Y = 30;
	Music0->AttenuationOverrides.FalloffDistance = 225;
}

// Called when the game starts or when spawned
void ATBSRadio::BeginPlay () {
	Super::BeginPlay ();

	Music0->Play ();
}

// Called every frame
void ATBSRadio::Tick (float DeltaTime) {
	Super::Tick (DeltaTime);

}

