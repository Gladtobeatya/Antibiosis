// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTest.h"
#include "EnemyAttackBase.h"

AEnemyTest::AEnemyTest()
{
	MaxHealth = 150.f;
	CharacterSpeed = 2.0f;
}

void AEnemyTest::PerformTurnAction_Implementation()
{
	Super::PerformTurnAction_Implementation();
	
	//TODO add implementation
	if (AttackInstances.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyTest randomly attacks"));
		const int32 Index = FMath::RandRange(0, AttackInstances.Num() - 1);
		//TODO Using this for new but need to get the right target
		AttackInstances[Index]->StartAttackPattern(this, PlayerCombatRef);
	}
}

void AEnemyTest::OnMyTurnStart_Implementation()
{
	Super::OnMyTurnStart_Implementation();
}
