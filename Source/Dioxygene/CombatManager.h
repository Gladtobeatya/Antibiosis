// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

UENUM()
enum class ECombatTurn : uint8
{
	Player,
	Enemy
};

DECLARE_DYNAMIC_DELEGATE(FOnPlayerTurnStarted);

class ADioxygeneCharacter;
class AEnemyCharacter;

UCLASS()
class DIOXYGENE_API ACombatManager : public AActor
{
	GENERATED_BODY()

private:
	//Ref to the player
	UPROPERTY()
	ADioxygeneCharacter* PlayerRef;

	//Ref to all enemies in this battle
	UPROPERTY()
	TArray<AEnemyCharacter*> EnemiesRef;

	UPROPERTY()
	TArray<AEnemyCharacter*> EnemiesDead;

	UPROPERTY()
	TArray<AEnemyCharacter*> EnemiesAlive;

	//Idx of the current enemy's turn
	int32 CurrentEnemyIndex = 0;

	//List of available targets
	UPROPERTY()
	TArray<AEnemyCharacter*> AvailableTargets;

	//When using a spell, this is the idx of the target the player has selected
	int32 CurrentTargetIndex = 0;

	//Actual character the CurrentTargetIndex is referring to
	UPROPERTY()
	AEnemyCharacter* CurrentTarget;

	//TODO, actor to manager which enemy is currently targeted
	UPROPERTY()
	AActor* TargetMarkerActor;
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	ECombatTurn CurrentTurn;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void InitCombat(ADioxygeneCharacter* Player, TArray<AEnemyCharacter*> Enemies);
	void StartCombat();

	void StartPlayerTurn();
	UFUNCTION()
	void OnPlayerTurnEnded();

	void StartEnemyTurn();
	
	UFUNCTION()
	void OnEnemyTurnEnded();

	void CheckCombatEnd();

	void EndCombat(bool bIsWon);

	UFUNCTION()
	void ApplyDamageToEnemy(const int32 TargetIndex, const float Damage);

	UFUNCTION(BlueprintCallable, Category="Combat")
	TArray<AEnemyCharacter*> GetAliveEnemies();

	UFUNCTION(BlueprintCallable, Category="Combat")
	TArray<AEnemyCharacter*> GetDeadEnemies();

};
