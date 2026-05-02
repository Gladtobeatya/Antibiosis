// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"
#include "DioxygeneCharacter.h"
#include "EnemyCharacter.h"

// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatManager::InitCombat(ADioxygeneCharacter* Player, TArray<AEnemyCharacter*> Enemies)
{
	PlayerRef = Player;
	EnemiesRef = Enemies;
	EnemiesAlive = EnemiesRef;
	EnemiesDead.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Combat initialized between %s and %i enemies"), 
		*Player->GetName(), Enemies.Num());

	Player->SetCombatManager(this);
	Player->OnPlayerTurnEnded.AddDynamic(this, &ACombatManager::OnPlayerTurnEnded);
	for(auto const &Enemy : EnemiesRef)
	{
		Enemy->OnEnemyTurnEnded.AddDynamic(this, &ACombatManager::OnEnemyTurnEnded);
	}
	StartCombat();
}

void ACombatManager::StartCombat()
{
	CurrentTurn = ECombatTurn::Player;
	if(PlayerRef && EnemiesRef.Num())
	{
		StartPlayerTurn();
	}
}

void ACombatManager::StartPlayerTurn()
{
	//do things before the player starts its turn
	//Call blueprint implementation
	PlayerRef->OnMyTurnStart();
}

void ACombatManager::OnPlayerTurnEnded()
{
	CheckCombatEnd();
	StartEnemyTurn();
}

void ACombatManager::StartEnemyTurn()
{
	EnemiesRef[CurrentEnemyIndex]->SetPlayerCombatRef(PlayerRef);
	EnemiesRef[CurrentEnemyIndex]->OnMyTurnStart();
}

void ACombatManager::OnEnemyTurnEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("cbtmanager Enemyturn ended"));
	CheckCombatEnd();
	//increments id till max value then loop
	CurrentEnemyIndex = (CurrentEnemyIndex + 1) % EnemiesRef.Num();
	StartPlayerTurn();
}

void ACombatManager::CheckCombatEnd()
{
	if (!PlayerRef || PlayerRef->IsDead())
	{
		//Lost combat
		EndCombat(false);
		return;
	}

	bool bAllEnemiesDead = true;
	for (const auto &Enemy : EnemiesRef)
	{
		if (Enemy && !Enemy->IsDead())
		{
			//Combat continues
			bAllEnemiesDead = false;
			break;
		}
	}

	if (bAllEnemiesDead)
	{
		//Combat won
		EndCombat(true);
	}
}

void ACombatManager::EndCombat(bool bIsWon)
{
	for(const auto &Enemy : EnemiesRef)
	{
		Enemy->OnEnemyTurnEnded.RemoveDynamic(this, &ACombatManager::OnEnemyTurnEnded);
	}
	// TODO : Apply rewards or game over based on bPlayerWon
	UE_LOG(LogTemp, Warning, TEXT("Combat ended!"));
	
	if(APlayerStateFfa* PS = Cast<APlayerStateFfa>(PlayerRef->GetPlayerState()))
	{
		PS->TriggerExploration();
	}
	// Clean / destroy
	Destroy();
}

void ACombatManager::ApplyDamageToEnemy(const int32 TargetIndex, const float Damage)
{
	if(!EnemiesRef.IsValidIndex(TargetIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatManagercpp : Invalid enemy index"))
		return;
	}
	if(AEnemyCharacter* EnemyTarget = EnemiesRef[TargetIndex])
	{
		EnemyTarget->ReceiveDamage(Damage);
	}
}

TArray<AEnemyCharacter*> ACombatManager::GetAliveEnemies()
{
	return EnemiesAlive;
}

TArray<AEnemyCharacter*> ACombatManager::GetDeadEnemies()
{
	return EnemiesDead;
}
