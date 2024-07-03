// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoiceChat.h"
#include "Components/ActorComponent.h"
#include "VoiceChatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIOXYGENE_API UVoiceChatComponent : public UActorComponent, public IVoiceChat
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVoiceChatComponent();

	virtual void StartRecordingVoiceChat() override;
	virtual void StopRecordingVoiceChat() override;
	virtual EVoiceResult RecordVoiceChat() override;
	virtual void TransmitVoiceChat(const int Channel = 0) override;
	virtual bool ReceiveVoiceChat(const int Channel = 0) override;
	virtual void PlayVoiceChat(UAudioComponent*) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<uint8> BuffRecorded;
	uint32 BuffRecordedSize;
	uint32 NBytesRecorded;
	TArray<uint8> BuffSend;
	uint32 BuffSendSize;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
