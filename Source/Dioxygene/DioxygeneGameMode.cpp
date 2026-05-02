// Copyright Epic Games, Inc. All Rights Reserved.

#include "DioxygeneGameMode.h"

#include <memory>

#include "CombatEncounter.h"
#include "DioxygeneCharacter.h"
#include "PlayerStateFfa.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AGameModeFFA::HandlePlayingPhaseChanged(const APlayerStateFfa* PlayerState, const EPlayingPhase NewPhase)
{
	if (NewPhase == EPlayingPhase::Combat && PlayerState && PlayerState->EnemyInCombat)
	{
		if(ADioxygeneCharacter* Player = Cast<ADioxygeneCharacter>(PlayerState->GetPawn()))
		{
			
			//UE_LOG(LogTemp, Warning, TEXT("GameMode : Player OK"));
			//CombatEncounter knows what enemy/ies to spawn
			if(ACombatEncounter* CombatEncounter = PlayerState->EnemyInCombat->ParentEncounter)
			{
				//UE_LOG(LogTemp, Warning, TEXT("GameMode : CombatEncounter found, next is trigger combat"));
				CombatEncounter->TriggerCombat(Player);
			}
		}
	}
}

void AGameModeFFA::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(APlayerStateFfa* PS = Cast<APlayerStateFfa>(NewPlayer->PlayerState))
	{
		PS->OnPlayingPhaseChanged.AddUObject(this, &AGameModeFFA::HandlePlayingPhaseChanged);
	}
	// Ensure the PlayerController and PlayerState are valid
	/*if (NewPlayer)
	{
		if (APlayerStateFfa* PlayerState = Cast<APlayerStateFfa>(NewPlayer->PlayerState))
		{
			// Set the PlayerSteamID
			PlayerState->InitSteamID();
			UE_LOG(LogTemp, Warning, TEXT("OnPostLogin  : steam ID %s"),
				*FString::Printf(TEXT("%llu"), PlayerState->GetSteamID().ConvertToUint64()));
		}
	}*/
}

FTransform AGameModeFFA::FindRandomPlayerStart()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	if(const AActor* RandomPLayerStart = PlayerStarts[UKismetMathLibrary::RandomIntegerInRange(0, PlayerStarts.Num() - 1)])
		return RandomPLayerStart->GetActorTransform();
	return FTransform();
}
