// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS.h"
#include "StateComponent.h"


// Sets default values for this component's properties
UStateComponent::UStateComponent() {
	bWantsInitializeComponent = true;

	// ...
}

// Called when the game starts
void UStateComponent::InitializeComponent() {
	Super::InitializeComponent();

	// ...

}

void UStateComponent::Reset() {
	RawStates.Empty();
}

void UStateComponent::ResizeStateArray(uint8 newSize) {
	while (RawStates.Num() < newSize) {
		RawStates.Add(false);
	}
}

bool UStateComponent::GetState(uint8 offset) {
	if (RawStates.Num() < (offset + 1)) {
		ResizeStateArray(offset + 1);
	}

	return RawStates[offset];
}

void UStateComponent::SetState(uint8 offset, bool value) {
	if (RawStates.Num() < (offset + 1)) {
		ResizeStateArray(offset + 1);
	}

	RawStates[offset] = value;
}

void UStateComponent::ToggleState(uint8 offset) {
	SetState(offset, GetState(offset));
}

void UStateComponent::SetToState(uint8 offset) {
	Reset();
	SetState(offset, true);
}
