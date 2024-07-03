// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateFfa.h"

#include "Net/UnrealNetwork.h"

void APlayerStateFfa::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PC = GetPlayerController(); PC && PC->IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState  : begin play"));
		InitSteamID();
	}
}

void APlayerStateFfa::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerStateFfa, PlayerSteamID);
}

void APlayerStateFfa::InitSteamID()
{
	if(SteamAPI_Init() && SteamUser())
	{
		//Declaring another FString variable because the member can be changed by the server since it's replicated
		FString const NewSteamID = FString::Printf(TEXT("%llu"), SteamUser()->GetSteamID().ConvertToUint64());
		//UE_LOG(LogTemp, Warning, TEXT("PlayerState  : steamid avt envoi : %s"), *NewSteamID);

		SV_RPCSetSteamID(NewSteamID);
	}
}

CSteamID APlayerStateFfa::GetSteamID() const
{
	return CSteamID(FCString::Strtoui64(*PlayerSteamID, nullptr, 10));;
}

void APlayerStateFfa::SV_RPCSetSteamID_Implementation(const FString& SteamID)
{
	PlayerSteamID = SteamID;
}
