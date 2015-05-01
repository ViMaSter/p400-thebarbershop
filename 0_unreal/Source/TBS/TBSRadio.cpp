// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "TBSRadio.h"


// Sets default values
ATBSRadio::ATBSRadio () {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create a camera boom...
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo (RootComponent);

	AudioComponentOffset = CreateDefaultSubobject<USceneComponent> (TEXT ("AudioComponentOffset"));
	AudioComponentOffset->AttachTo (RootComponent);
	AudioComponentOffset->RelativeLocation = FVector (0, 0, 37.75);
	AudioComponentOffset->RelativeRotation = FRotator (0, -90.0, 0);

	Music0 = CreateDefaultSubobject<UAudioComponent> (TEXT ("Music0"));
	Music0->AttachTo (AudioComponentOffset);
	Music0->bOverrideAttenuation = true;
	Music0->bAllowSpatialization = true;
	Music0->OnAudioFinished.AddDynamic (this, &ATBSRadio::AudioFinished0);

	Music1 = CreateDefaultSubobject<UAudioComponent> (TEXT ("Music1"));
	Music1->AttachTo (AudioComponentOffset);
	Music1->bOverrideAttenuation = true;
	Music1->bAllowSpatialization = true;
	Music1->OnAudioFinished.AddDynamic (this, &ATBSRadio::AudioFinished1);

	ChannelSwitchNoise = CreateDefaultSubobject<UAudioComponent> (TEXT ("ChannelSwitchNoise"));
	ChannelSwitchNoise->AttachTo (AudioComponentOffset);
	ChannelSwitchNoise->bOverrideAttenuation = true;
	ChannelSwitchNoise->bAllowSpatialization = true;

	NewIs1 = true;
	CurrentStation = 0;

	PlayMusic = true;
}

// Called when the game starts or when spawned
void ATBSRadio::BeginPlay () {
	Super::BeginPlay ();

	if (PlayMusic) {
		Music0->Play ();
	}
}

// Called every frame
void ATBSRadio::Tick (float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ATBSRadio::AudioFinished0 () {
	if (ChannelToFadeIn == Music0) {
		Music0->Sound = RadioStations[CurrentStation].NextTrack ();
		Music0->Play (0.0f);
	}
}

void ATBSRadio::AudioFinished1 () {
	if (ChannelToFadeIn == Music1) {
		Music1->Sound = RadioStations[CurrentStation].NextTrack ();
		Music1->Play (0.0f);
	}
}

void ATBSRadio::SwitchStation (uint8 direction) {
	CurrentStation += direction;
	while (CurrentStation < 0) {
		CurrentStation += RadioStations.Num ();
	}

	CurrentStation %= RadioStations.Num ();

	UAudioComponent* CurrentComponent = NewIs1 ? Music0 : Music1;
	UAudioComponent* NewComponent = NewIs1 ? Music1 : Music0;
	NewComponent->Sound = RadioStations[CurrentStation].NextTrack ();

	ChannelToFadeIn = NewComponent;

	CurrentComponent->FadeOut (0.5f, 0.0f);
	ChannelSwitchNoise->Play ();
	ChannelToFadeIn->Stop ();
	ChannelToFadeIn->FadeIn (
		0.5f,
		1.0f,
		FMath::Fmod (GetWorld ()->TimeSeconds, ChannelToFadeIn->Sound->Duration)
		);
	NewIs1 = !NewIs1;
}

void ATBSRadio::SwitchStationFadeIn () {
	GetWorldTimerManager ().ClearTimer (ChannelFadeIn);
	
}