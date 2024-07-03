// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateFfa.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Sound/SoundWaveProcedural.h"
#include "DioxygeneCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADioxygeneCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** AudioComponent for mic testing purpose */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

	UPROPERTY()
	USoundWaveProcedural* VoiceSoundWave;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Talk Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* TalkAction;

	/** Handles the proximity chat bounds : others overlapping can hear player voice */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphereProximity;
	
	/* Check if steam API has already and successfully been initialized */
	bool IsSteamOK;

	/* Voice members declarations, use it in tandem with receiveVoice*/
	bool IsVoiceBeingRecorded;
	// Initial buffer size
	uint32 InitialBufferSize;
	// Initial buffer
	TArray<uint8> DecompressedBuffer;
	//Actual data size written into the buffer
	uint32 DecompBuffSignificantSize;
	// SampleRate used in decompression and AudioSoundWave
	uint32 SampleRate;

	//Avoid going through every player in the game every time we receive data from the same player (ex : ReceiveVoice)
	UPROPERTY()
	APlayerStateFfa* LastPlayerStateFound;
	
public:
	ADioxygeneCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for talk input */
	void Talk();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
	// End of APawn interface

	//Starts recording voice and send it to every user in the lobby
	void SendVoice();

	//Receives voice data and write it in DecompressedBuffer member
	void ReceiveVoice(CSteamID* SenderSteamID);

	APlayerStateFfa* FindPlayerStateBySteamID(const CSteamID SteamID);

	void PlayVoiceDataOnPlayer(const ADioxygeneCharacter* Player);
	

public:
	//Use for replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UAudioComponent* GetAudioComponent() const { return AudioComponent;}

	USoundWaveProcedural* GetVoiceSoundWave() const { return VoiceSoundWave;}

	/** Called every frame **/
	virtual void Tick( float DeltaSeconds ) override;

	//Used for proximity chat : we only send data to people overlapping with collisionSphereProximity
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ProxChat")
	TArray<ADioxygeneCharacter*> OverlappingCharacters;
	
};

