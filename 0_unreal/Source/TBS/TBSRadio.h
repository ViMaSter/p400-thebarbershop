// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TBSRadio.generated.h"

UCLASS()
class TBS_API ATBSRadio : public AActor {
	GENERATED_BODY ()

public:
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* Music0;


	// Sets default values for this actor's properties
	ATBSRadio ();

	// Called when the game starts or when spawned
	virtual void BeginPlay () override;

	// Called every frame
	virtual void Tick (float DeltaSeconds) override;



};
