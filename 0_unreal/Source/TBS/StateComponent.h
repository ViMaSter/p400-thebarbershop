// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TBS_API UStateComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UStateComponent();

	TArray<bool> RawStates;
	void ResizeStateArray(uint8 newSize);

	UFUNCTION(BlueprintCallable, Category = "State") void Reset();
	UFUNCTION(BlueprintCallable, Category = "State") bool GetState(uint8 offset);
	UFUNCTION(BlueprintCallable, Category = "State") void SetState(uint8 offset, bool value);
	UFUNCTION(BlueprintCallable, Category = "State") void ToggleState(uint8 offset);
	UFUNCTION(BlueprintCallable, Category = "State") void SetToState(uint8 offset);
};
