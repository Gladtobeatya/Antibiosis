// Fill out your copyright notice in the Description page of Project Settings.


#include "OxygenComponent.h"

// Sets default values for this component's properties
UOxygenComponent::UOxygenComponent()
{
	MaxOxygen = 1000;
	CurrentOxygen = MaxOxygen;
	DecayRate = 1;
	IsDecayActive = true;
}


// Called when the game starts
void UOxygenComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(OxygenDecayTimerHandle, this, &UOxygenComponent::UpdateOxygen, 0.1f, true);
	
}


// Called every frame
void UOxygenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UOxygenComponent::GetMaxOxygen() const
{
	return MaxOxygen;
}

int UOxygenComponent::GetCurrentOxygen() const
{
	return CurrentOxygen;
}

void UOxygenComponent::SetOxygen(const int Amount)
{
	CurrentOxygen = Amount;
}

void UOxygenComponent::SetDecayRate(const int Amount)
{
	DecayRate = Amount;
}

void UOxygenComponent::SetIsDecayActive(const bool NewIsActive)
{
	IsDecayActive = NewIsActive;
}

void UOxygenComponent::IncreaseOxygen(const int Amount)
{
	CurrentOxygen = FMath::Min(MaxOxygen, CurrentOxygen + Amount);
}

void UOxygenComponent::DecreaseOxygen(const int Amount)
{
	CurrentOxygen = FMath::Max(0, CurrentOxygen - Amount);
}

void UOxygenComponent::UpdateOxygen()
{
	if(IsDecayActive)
		DecreaseOxygen(DecayRate);
}

