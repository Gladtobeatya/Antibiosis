// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DioxygeneCharacter.h"
#include "EnemyCharacter.generated.h"

class AEnemyAttackBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyTurnEnded);

UCLASS()
class DIOXYGENE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CharacterSpeed = 1.f;
	
	//Attacks the enemy can use, editable dynamically
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spell")
	TArray<TSubclassOf<AEnemyAttackBase>> AvailableAttacks;

	//Instances of the attacks
	UPROPERTY()
	TArray<AEnemyAttackBase*> AttackInstances;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat")
	void OnMyTurnStart();
	virtual void OnMyTurnStart_Implementation();

	//Attack pattern, override this or configure with data
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat")
	void PerformTurnAction();
	virtual void PerformTurnAction_Implementation();
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	virtual void ReceiveDamage(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, Category="Combat")
	void Die();
	virtual void Die_Implementation();

	// Called when anim/attack is done, and the turn ended
	UPROPERTY(BlueprintAssignable)
	FOnEnemyTurnEnded OnEnemyTurnEnded;

	UPROPERTY()
	ADioxygeneCharacter* PlayerCombatRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetPlayerCombatRef(ADioxygeneCharacter* InPlayerCombatRef);

private:
	UPROPERTY()
	bool bIsDead = false;
public:
	UFUNCTION()
	bool IsDead();

};
