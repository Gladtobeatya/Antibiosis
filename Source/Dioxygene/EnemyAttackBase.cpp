// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackBase.h"

#include "CombatTypes.h"
#include "DioxygeneCharacter.h"
#include "EnemyCharacter.h"

void AEnemyAttackBase::StartAttackPattern(ACharacter* Caster, ACharacter* Target)
{
	if (AttackPattern.Num() == 0 || !Caster || !Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAtkBase: No attack events or invalid caster/target"));
		return;
	}

	//Start at idx 0 for first atk
	ExecuteNextAttack(Caster, Target);
}

void AEnemyAttackBase::ExecuteNextAttack(ACharacter* Caster, ACharacter* Target)
{
	if(CurrIndex >= AttackPattern.Num())
	{
		return;
	}
	
	const FAttackPart& Attack = AttackPattern[CurrIndex];

	//Call to BP for visuals
	OnAttackStart(Caster, Target);

	// Handle = manage timer
	FTimerHandle HitHandle;
	// Handle = manage timer
	FTimerHandle NextAttackHandle;
	
	// Timer for the hit moment (delegate = do stuff each interval)
	FTimerDelegate HitDelegate;
	HitDelegate.BindLambda([=, this]()
	{
		if (Target && Caster)
		{
			UE_LOG(LogTemp, Warning, TEXT("EnAtkBase: HitDelegate!"));
			if(ADioxygeneCharacter* Character = Cast<ADioxygeneCharacter>(Target))
			{
				switch(Character->CheckParry(Attack.PerfectParryWindow))
				{
				case EParryResult::NoParry:
					UE_LOG(LogTemp, Warning, TEXT("EnemyAttackBase: No parry"));
					Character->ReceiveDamage(Attack.Damage);
					break;
				case EParryResult::NormalParry:
					UE_LOG(LogTemp, Warning, TEXT("EnemyAttackBase: Normal parry"));
					break;
				case EParryResult::PerfectParry:
					UE_LOG(LogTemp, Warning, TEXT("EnemyAttackBase: Perfect parry!"));
					break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("EnemyAttackBase: No parry result!"));
				}
			}
			//TODO Apply damage
			//Call to BP for visuals
			OnAttackHit(Caster, Target);
			
		}
	});
	GetWorld()->GetTimerManager().SetTimer(HitHandle, HitDelegate, Attack.HitTiming, false);

	// Timer pour passer à la prochaine attaque
	FTimerDelegate NextAttackDelegate;
	NextAttackDelegate.BindLambda([=, this]()
	{
		OnAttackEnd();
		UE_LOG(LogTemp, Warning, TEXT("EnAtkBase: Hit ended, next one!"));
		CurrIndex++;
		if(CurrIndex >= AttackPattern.Num())
		{
			if(AEnemyCharacter *CurrEnemy = Cast<AEnemyCharacter>(Caster))
			{
				CurrIndex = 0;
				CurrEnemy->OnEnemyTurnEnded.Broadcast();
			}
		}
		else
		{
			ExecuteNextAttack(Caster, Target);
		}
	});

	GetWorld()->GetTimerManager().SetTimer(
		NextAttackHandle, NextAttackDelegate, Attack.Duration, false
	);
	
}
