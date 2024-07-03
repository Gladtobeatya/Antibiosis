// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
THIRD_PARTY_INCLUDES_START

#include <steam/steam_api.h>

THIRD_PARTY_INCLUDES_END
#include "PlayerStateFfa.generated.h"

/**
 * 
 */
UCLASS()
class DIOXYGENE_API APlayerStateFfa : public APlayerState
{
	GENERATED_BODY()

public :

	virtual void BeginPlay() override;
	
	//Using FString to be able to replicate it (UE doesn't know about CSteamID
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	FString PlayerSteamID;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitSteamID();
	CSteamID GetSteamID() const;

	//Server RPC to set the steamID on the server
	UFUNCTION(Server, Reliable)
	void SV_RPCSetSteamID(const FString & SteamID);
	
	
};
