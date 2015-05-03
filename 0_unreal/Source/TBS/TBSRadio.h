// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSRadio.generated.h"

USTRUCT (BlueprintType)
struct FTBSRadioStation {
	GENERATED_USTRUCT_BODY ()

public:
	FTBSRadioStation () : CurrentTrack(0) {}
	uint32 CurrentTrack;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Audio)
	TArray<USoundBase*> MusicPieces;

	USoundBase* NextTrack () {
		USoundBase* Track = MusicPieces[CurrentTrack];

		CurrentTrack += 1;
		CurrentTrack %= MusicPieces.Num ();

		return Track;
	}
};

UCLASS()
class TBS_API ATBSRadio : public AActor {
	GENERATED_BODY ()

public:
	// Component
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* AudioComponentOffset;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* Music0;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* Music1;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* ChannelSwitchNoise;
	UAudioComponent* ChannelToFadeIn;
	FTimerHandle ChannelFadeIn;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	TArray<FTBSRadioStation> RadioStations;
	bool NewIs1;
	int CurrentStation;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Audio)
	bool PlayMusic;

	// Sets default values for this actor's properties
	ATBSRadio ();

	// Called when the game starts or when spawned
	UFUNCTION (BlueprintCallable, Category = Radio)
	void SwitchStation (int32 direction);
	void SwitchStationFadeIn ();

	UFUNCTION (Category = Radio)
	void AudioFinished0 ();
	UFUNCTION (Category = Radio)
	void AudioFinished1 ();

	// Called when the game starts or when spawned
	virtual void BeginPlay () override;

	// Tick helper
	void ApplyFrequency (float DeltaTime);
	// Called every frame
	virtual void Tick (float DeltaSeconds) override;



};
