// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceChatComponent.h"

// Sets default values for this component's properties
UVoiceChatComponent::UVoiceChatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	BuffRecordedSize = 1024;
	BuffRecorded.AddZeroed(BuffRecordedSize);
	NBytesRecorded = 0;
}

void UVoiceChatComponent::StartRecordingVoiceChat()
{
	if(SteamUser())
	{
		SteamUser()->StartVoiceRecording();
	}
}

void UVoiceChatComponent::StopRecordingVoiceChat()
{
	if(SteamUser())
	{
		SteamUser()->StopVoiceRecording();
	}
}

EVoiceResult UVoiceChatComponent::RecordVoiceChat()
{
	uint32 PcbCompressed;
	const EVoiceResult Result = SteamUser()->GetAvailableVoice(&PcbCompressed);
	if(PcbCompressed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tick : pcbCompressed ok"));
		return SteamUser()->GetVoice(true, BuffRecorded.GetData(), BuffRecordedSize, &NBytesRecorded);
	}
	return Result;
}

void UVoiceChatComponent::TransmitVoiceChat(const int Channel)
{
}

bool UVoiceChatComponent::ReceiveVoiceChat(const int Channel)
{
	// Vérifier s'il y a un paquet P2P disponible
	uint32 PacketSize;
	CSteamID SenderSteamID;

	if (SteamNetworking()->IsP2PPacketAvailable(&PacketSize))
	{
		static TArray<uint8> Packet;
		Packet.SetNum(PacketSize);
		Packet.AddZeroed(PacketSize);
		return SteamNetworking()->ReadP2PPacket(Packet.GetData(), PacketSize, &PacketSize, &SenderSteamID);
	}
	
	return false;
}

void UVoiceChatComponent::PlayVoiceChat(UAudioComponent* AudioComponent)
{
}


// Called when the game starts
void UVoiceChatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVoiceChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

