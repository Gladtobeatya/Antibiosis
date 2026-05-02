// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DioxygeneCharacter.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "CombatEncounter.generated.h"

/* Instantiate this class for each combat */
UCLASS()
class DIOXYGENE_API ACombatEncounter : public AActor
{
	GENERATED_BODY()

private:
	bool bHasBeenTriggered = false;
public:

	UPROPERTY(EditInstanceOnly)
	TArray<TSubclassOf<AEnemyCharacter>> Enemies;

	UFUNCTION()
	TArray<AEnemyCharacter*> SpawnEnemies();
	
	void TriggerCombat(ADioxygeneCharacter* Player);
	
	// Sets default values for this actor's properties
	ACombatEncounter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
