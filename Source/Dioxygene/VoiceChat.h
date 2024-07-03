// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
THIRD_PARTY_INCLUDES_START

#include <steam/steam_api.h>

THIRD_PARTY_INCLUDES_END
#include "VoiceChat.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVoiceChat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIOXYGENE_API IVoiceChat
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartRecordingVoiceChat() = 0;
	virtual void StopRecordingVoiceChat() = 0;
	virtual EVoiceResult RecordVoiceChat() = 0;
	virtual void TransmitVoiceChat(const int Channel = 0) = 0;
	virtual bool ReceiveVoiceChat(const int Channel = 0) = 0;
	virtual void PlayVoiceChat(UAudioComponent*) = 0;
};
