// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyDummy.h"
#include "GameFramework/PlayerState.h"
THIRD_PARTY_INCLUDES_START

#include <steam/steam_api.h>

THIRD_PARTY_INCLUDES_END
#include "PlayerStateFfa.generated.h"

/*Player current playing phase.
 *UMETA to change some meta settings
 *Count, so we can iterate easily in c++, example :
 *for(EPlayingPhase PlayingPhase : TEnumRange<EPlayingPhase>())
 *{
 *} 
*/
UENUM(BlueprintType)
enum class EPlayingPhase : uint8
{
	Exploration UMETA(DisplayName = "Exploration"),
	Combat UMETA(DisplayName = "Combat"),
	Cinematic UMETA(DisplayName = "Cinematic"),
	Count UMETA(Hidden),
};
ENUM_RANGE_BY_COUNT(EPlayingPhase, EPlayingPhase::Count);

//For the delegate only
class APlayerStateFfa;
// Delegate C++ pur
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayingPhaseChangedDelegate, const APlayerStateFfa*, const EPlayingPhase);
/**
 * 
 */
UCLASS()
class DIOXYGENE_API APlayerStateFfa : public APlayerState
{
	GENERATED_BODY()

public :

	APlayerStateFfa();
	virtual void BeginPlay() override;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Player")
	EPlayingPhase CurrentPlayingPhase;

	UPROPERTY()
	AEnemyDummy* EnemyInCombat;

	//-- Begin PlayingPhase
	UFUNCTION(BlueprintCallable)
	void SetPlayingPhase(const EPlayingPhase PlayingPhase);

	// Delegate C++
	FOnPlayingPhaseChangedDelegate OnPlayingPhaseChanged;

	// Event for the Blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void OnPlayingPhaseChanged_BP(const APlayerStateFfa* PlayerState, const EPlayingPhase NewPhase);
		//-- Begin Combat phase
	
	void TriggerCombat(AEnemyDummy* InEnemy);
	
		//-- End Combat phase

	void TriggerExploration();
	//-- End PlayingPhase
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//-- Begin Steamwork things
	//Using FString to be able to replicate it (UE doesn't know about CSteamID
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	FString PlayerSteamID;


	void InitSteamID();
	CSteamID GetSteamID() const;

	//Server RPC to set the steamID on the server
	UFUNCTION(Server, Reliable)
	void SV_RPCSetSteamID(const FString & SteamID);

	//-- End Steamwork things
	
	
};
