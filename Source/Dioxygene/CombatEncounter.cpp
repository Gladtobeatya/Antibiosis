// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEncounter.h"
#include "CombatManager.h"
#include "DioxygeneGameMode.h"

TArray<AEnemyCharacter*> ACombatEncounter::SpawnEnemies()
{
	TArray<AEnemyCharacter*> SpawnedEnemies;

	UE_LOG(LogTemp, Warning, TEXT("CombatEncounter : SpawnEnemies"));
	for(const TSubclassOf<AEnemyCharacter>& Enemy : Enemies)
	{
		if(!Enemy)
		{
			continue;
		}
		//For testing purpose
		FVector SpawnLocation = GetActorLocation() + FMath::VRand() * 200.f; 
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (AEnemyCharacter* Spawned = GetWorld()->SpawnActor<AEnemyCharacter>(Enemy, SpawnLocation, SpawnRotation))
		{
			SpawnedEnemies.Add(Spawned);
		}
	}
	
	return SpawnedEnemies;
}

void ACombatEncounter::TriggerCombat(ADioxygeneCharacter* Player)
{
	if (bHasBeenTriggered)
		return;

	bHasBeenTriggered = true;

	// Call GameMode to create combat
	if (AGameModeFFA* GameMode = Cast<AGameModeFFA>(GetWorld()->GetAuthGameMode()))
	{
		TArray<AEnemyCharacter*> SpawnedEnemies = SpawnEnemies();
		ACombatManager* CombatManager = GetWorld()->SpawnActor<ACombatManager>(ACombatManager::StaticClass());
		CombatManager->InitCombat(Player, SpawnedEnemies);
	}
}

// Sets default values
ACombatEncounter::ACombatEncounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatEncounter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatEncounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

