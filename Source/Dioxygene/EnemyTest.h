// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "EnemyTest.generated.h"

class AEnemyAttackBase;
/**
 * 
 */
UCLASS()
class DIOXYGENE_API AEnemyTest : public AEnemyCharacter
{
	GENERATED_BODY()

public :
	AEnemyTest();

	virtual void PerformTurnAction_Implementation() override;
	virtual void OnMyTurnStart_Implementation() override;
		
};
