// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OxygenComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIOXYGENE_API UOxygenComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UOxygenComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	int MaxOxygen;

	UPROPERTY(EditAnywhere)
	int CurrentOxygen;

	UPROPERTY(EditAnywhere)
	int DecayRate;

	UPROPERTY(EditAnywhere)
	FTimerHandle OxygenDecayTimerHandle;

	UPROPERTY(EditAnywhere)
	bool IsDecayActive;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	int GetMaxOxygen() const;

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	int GetCurrentOxygen() const;

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	void SetOxygen(const int Amount);

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	void SetDecayRate(const int Amount);

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	void SetIsDecayActive(const bool NewIsActive);

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	void IncreaseOxygen(const int Amount);

	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	void DecreaseOxygen(const int Amount);

	void UpdateOxygen();
	
};
