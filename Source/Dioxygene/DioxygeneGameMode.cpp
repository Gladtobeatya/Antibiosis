// Copyright Epic Games, Inc. All Rights Reserved.

#include "DioxygeneGameMode.h"

#include <memory>

#include "PlayerStateFfa.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AGameModeFFA::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

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
