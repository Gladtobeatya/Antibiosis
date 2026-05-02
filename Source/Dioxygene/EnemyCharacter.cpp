// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyAttackBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::OnMyTurnStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyTurnStarted"));
	//Override this in child class or BP
	PerformTurnAction();
}

void AEnemyCharacter::PerformTurnAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy performs a generic action"))
	//Override this in child class or BP
}

void AEnemyCharacter::ReceiveDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter : Health after dmg taken : %f"), CurrentHealth);
	if(CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		Die();
	}
}

void AEnemyCharacter::Die_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy died."))
	bIsDead = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	for (auto& AttackClass : AvailableAttacks)
	{
		if (AttackClass)
		{
			if (AEnemyAttackBase* AttackInstance = NewObject<AEnemyAttackBase>(this, AttackClass))
			{
				AttackInstance->OwnerPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				AttackInstances.Add(AttackInstance);
			}
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::SetPlayerCombatRef(ADioxygeneCharacter* InPlayerCombatRef)
{
	PlayerCombatRef = InPlayerCombatRef;
}

bool AEnemyCharacter::IsDead()
{
	return bIsDead;
}

