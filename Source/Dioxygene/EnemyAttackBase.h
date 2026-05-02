// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyAttackBase.generated.h"


USTRUCT(BlueprintType)
struct FAttackPart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 2.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PerfectParryWindow = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitTiming = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.f;
};

/**
 * 
 */
UCLASS(Blueprintable)
class DIOXYGENE_API AEnemyAttackBase : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spell")
	FName AttackName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spell")
	TArray<FAttackPart> AttackPattern;

	UPROPERTY(BlueprintReadOnly, Category="Spell")
	int32 CurrIndex = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Spell")
	void StartAttackPattern(ACharacter* Caster, ACharacter* Target);

	UPROPERTY(BlueprintReadOnly)
	APlayerController* OwnerPlayer;

protected:
	/** Blueprint events for visual/animation feedback */
	UFUNCTION(BlueprintImplementableEvent, Category="Spell")
	void OnAttackStart(ACharacter* Caster, ACharacter* Target);

	UFUNCTION(BlueprintImplementableEvent, Category="Spell")
	void OnAttackHit(ACharacter* Caster, ACharacter* Target);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Spell")
	void OnAttackEnd();
	
	//Recursive call for every "attack" of the pattern. If the pattern consists of x attacks, this is called x times
	void ExecuteNextAttack(ACharacter* Caster, ACharacter* Target);	
};
