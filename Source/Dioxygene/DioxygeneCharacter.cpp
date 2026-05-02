// Copyright Epic Games, Inc. All Rights Reserved.

#include "DioxygeneCharacter.h"
#include "DioxygeneProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interface_Interact.h"
#include "PlayerStateFfa.h"
#include "CombatManager.h"
#include "CombatTypes.h"
#include "SpellTest.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADioxygeneCharacter

ADioxygeneCharacter::ADioxygeneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	GetMesh()->SetOwnerNoSee(true);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CharacterAudio"));
	AudioComponent->SetupAttachment(FirstPersonCameraComponent);
	AudioComponent->bAutoActivate = true;

	// Create a sphere component that will be used when this player talks so others can hear it too
	CollisionSphereProximity = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphereProximity"));
	CollisionSphereProximity->SetupAttachment(RootComponent);
	// 25 meters radius by default
	CollisionSphereProximity->SetSphereRadius(2000.0f);
	// TODO for debug purpose
	CollisionSphereProximity->SetHiddenInGame(false);

	
	IsSteamOK = false;

	IsVoiceBeingRecorded = false;
	//starting with 20 KB as suggested
	InitialBufferSize = 20 * 1024;
	// Allocate initial buffer
	DecompressedBuffer.Empty();
	DecompressedBuffer.AddZeroed(InitialBufferSize);
	DecompBuffSignificantSize = 0;
	SampleRate = 44100;

	//Initializes distance from the camera to 5 meters for line trace (used in tick)
	LineTraceDistance = 200.0f;
	
	MaxHealth = 100.f;
}

void ADioxygeneCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	
	if(SteamAPI_Init() && SteamUser())
	{
		IsSteamOK = true;
		//UE_LOG(LogTemp, Warning, TEXT("Begin play  : steam ini & user OK"));
	
		UE_LOG(LogTemp, Warning, TEXT("Begin play  : authority %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
		UE_LOG(LogTemp, Warning, TEXT("Begin play  : steamID %llu"), SteamUser()->GetSteamID().ConvertToUint64());
	}
	//Sets up voice for multi
	if(AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin play  : audio component OK"));
		VoiceSoundWave = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());
		VoiceSoundWave->SetSampleRate(SampleRate);  // Default sample rate
		VoiceSoundWave->NumChannels = 1;
		VoiceSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
		VoiceSoundWave->SoundGroup = SOUNDGROUP_Voice;
		VoiceSoundWave->bLooping = false;
	}
	
	// Bind overlap events for the sphere component
	if (CollisionSphereProximity)
	{
		CollisionSphereProximity->OnComponentBeginOverlap.AddDynamic(this, &ADioxygeneCharacter::OnOverlapBegin);
		CollisionSphereProximity->OnComponentEndOverlap.AddDynamic(this, &ADioxygeneCharacter::OnOverlapEnd);
	}	
}

//////////////////////////////////////////////////////////////////////////// Input

void ADioxygeneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADioxygeneCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADioxygeneCharacter::Look);

		// Voice chatting
		EnhancedInputComponent->BindAction(TalkAction, ETriggerEvent::Started, this, &ADioxygeneCharacter::Talk);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADioxygeneCharacter::Interact);
		
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ADioxygeneCharacter::StartParry);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Completed, this, &ADioxygeneCharacter::EndParry);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADioxygeneCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(!HasAuthority())
		return;
	
	// Check if overlapping actor is a ADioxygeneCharacter and not self
	if (ADioxygeneCharacter* OtherCharacter = Cast<ADioxygeneCharacter>(OtherActor))
	{
		if (Cast<UCapsuleComponent>(OtherComp) && OtherCharacter != this)
		{
			UE_LOG(LogTemp, Warning, TEXT("OCOB : ADD"));
			OverlappingCharacters.AddUnique(OtherCharacter);
		}
	}
}

void ADioxygeneCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!HasAuthority())
		return;

	// Check if the overlapping actor is another character
	if (ADioxygeneCharacter* OtherCharacter = Cast<ADioxygeneCharacter>(OtherActor))
	{
		if (Cast<UCapsuleComponent>(OtherComp))
		{
			UE_LOG(LogTemp, Warning, TEXT("OCOB : REMOVE"));
			OverlappingCharacters.Remove(OtherCharacter);
		}
	}
}

void ADioxygeneCharacter::SendVoice()
{

	//No execution on server side, except for the host
	if(!IsLocallyControlled() || OverlappingCharacters.IsEmpty())
	{
		SteamUser()->StopVoiceRecording();
		IsVoiceBeingRecorded = false;
		return;
	}
	if(!IsVoiceBeingRecorded)
	{
		SteamUser()->StartVoiceRecording();
		IsVoiceBeingRecorded = true;
	}
	uint32 PcbCompressed;
	SteamUser()->GetAvailableVoice(&PcbCompressed);
	if(PcbCompressed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tick : pcbCompressed size : %d"), PcbCompressed);
		TArray<uint8> PDestBuffer;
		const uint32 DestBufferSize = PcbCompressed;
		PDestBuffer.AddZeroed(DestBufferSize);
		uint32 NBytesWritten = 0;
		
		if(SteamUser()->GetVoice(true, PDestBuffer.GetData(), DestBufferSize, &NBytesWritten) == k_EVoiceResultOK)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Tick : get voice size %d"), PcbCompressed);
			//UE_LOG(LogTemp, Warning, TEXT("Tick : get voice sent %d"), NBytesWritten);
			for(const auto Player : OverlappingCharacters)
			{
				if(const APlayerStateFfa * CurrPlayerState = Cast<APlayerStateFfa>(Player->GetPlayerState()))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Talk  : for loop steam id : %s"), *CurrPlayerState->PlayerSteamID);
					SteamNetworking()->SendP2PPacket(CurrPlayerState->GetSteamID(), PDestBuffer.GetData(), NBytesWritten, k_EP2PSendUnreliable);
				}
			}
		}
	}
}

void ADioxygeneCharacter::ReceiveVoice(CSteamID* SenderSteamID)
{

	DecompBuffSignificantSize = 0;
	if(!IsLocallyControlled() || !SenderSteamID)
		return;
	
	uint32 PacketSize;

	// Verify if a P2P Packet is available
	if (SteamNetworking()->IsP2PPacketAvailable(&PacketSize))
	{
		TArray<uint8> Packet;
		Packet.SetNum(PacketSize);
		Packet.AddZeroed(PacketSize);
		// Read it
		if(SteamNetworking()->ReadP2PPacket(Packet.GetData(), PacketSize, &PacketSize, SenderSteamID))
		{			
			// Decompress the voice data
			SteamUser()->DecompressVoice(Packet.GetData(), PacketSize,DecompressedBuffer.GetData(),
				InitialBufferSize, &DecompBuffSignificantSize, SampleRate);
			//UE_LOG(LogTemp, Warning, TEXT("Receive  : decompressed size : %d"), DecompBuffSignificantSize);
		}
	}
}

APlayerStateFfa* ADioxygeneCharacter::FindPlayerStateBySteamID(const CSteamID SteamID)
{
	//Return the LastPlayerStateFound if steamID is the same as before (avoids for loop in tick)
	if(LastPlayerStateFound && LastPlayerStateFound->GetSteamID() == SteamID)
	{
		return LastPlayerStateFound;
	}
	LastPlayerStateFound = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Different person than last time"))
	TArray<TObjectPtr<APlayerState>> AllPS = GetWorld()->GetGameState()->PlayerArray;
	
	for(auto SenderPS : AllPS)
	{
		if(Cast<APlayerStateFfa>(SenderPS)->GetSteamID() == SteamID)
		{
			LastPlayerStateFound = Cast<APlayerStateFfa>(SenderPS);
			break;
		}
	}
	return LastPlayerStateFound;
}

void ADioxygeneCharacter::PlayVoiceDataOnPlayer(const ADioxygeneCharacter* Player)
{
	if(!Player)
		return;

	if(USoundWaveProcedural* PlayerVoiceSoundWave = Player->GetVoiceSoundWave())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Receive  : playVoice size : %d"), DecompBuffSignificantSize);
		// Queue audio data to the SoundWaveProcedural
		PlayerVoiceSoundWave->QueueAudio(DecompressedBuffer.GetData(), DecompBuffSignificantSize);
		DecompBuffSignificantSize = 0;
		// Ensure AudioComponent is valid and not already playing, then play the audio
		if (UAudioComponent* PlayerAudioComponent = Player->GetAudioComponent(); !PlayerAudioComponent->IsPlaying())
		{
			PlayerAudioComponent->SetSound(PlayerVoiceSoundWave);
			PlayerAudioComponent->Play();
		}	
	}
}

void ADioxygeneCharacter::VoiceChatTick()
{
	if(!IsSteamOK || !IsLocallyControlled())
		return;
	//UE_LOG(LogTemp, Warning, TEXT("Tick : steam ok"));
			
	//TODO REMOVE -- JUST FOR TESTING PURPOSE
	if(HasAuthority())
		SendVoice();
	CSteamID SenderSteamID;
	ReceiveVoice(&SenderSteamID);
	if(DecompBuffSignificantSize > 0)
	{
		if(const APlayerStateFfa* SenderPlayerState = FindPlayerStateBySteamID(SenderSteamID))
		{
			if(const ADioxygeneCharacter* SenderCharacter = Cast<ADioxygeneCharacter>(SenderPlayerState->GetPawn()))
			{
				PlayVoiceDataOnPlayer(SenderCharacter);
			}
		}
	}
}

void ADioxygeneCharacter::LineTraceTick()
{
	if(!IsLocallyControlled())
		return;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const TObjectPtr<APlayerCameraManager> CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	//End location is basically "LineTraceDistance" forward to player view
	const FVector EndTraceLocation = CameraManager->GetActorForwardVector() * LineTraceDistance + CameraManager->GetCameraLocation();

	//ECC_GameTraceChannel1 is "Interaction" trace channel
	
	if(FHitResult HitResult; GetWorld()->SweepSingleByChannel(
		HitResult, CameraManager->GetCameraLocation(), EndTraceLocation, FQuat::Identity, TraceChannelProperty, FCollisionShape::MakeSphere(20.0f), QueryParams))
	{
		// Draw debug line for visualization
		//DrawDebugLine(GetWorld(), CameraManager->GetCameraLocation(), HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 2.0f);
		if(const IInterface_Interact* InteractableActor = Cast<IInterface_Interact>(HitResult.GetActor()))
		{
			InteractActor = HitResult.GetActor();
			InteractableActor->Execute_SetFocused(InteractActor);
			return;
		}
	}
	
	//Reset value so we don't accidentally use it when it is not in focus anymore
	if(InteractActor)
		InteractActor = nullptr;
}

void ADioxygeneCharacter::Server_Interact_Implementation()
{
	
}

void ADioxygeneCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADioxygeneCharacter, OverlappingCharacters);
}

void ADioxygeneCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	LineTraceTick();
	VoiceChatTick();
}

bool ADioxygeneCharacter::IsDead()
{
	return bIsDead;
}

void ADioxygeneCharacter::OnMyTurnStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Diocharacter : Character turn started!"));
	
	//My turn ended
	//OnPlayerTurnEnded.Broadcast();
}

void ADioxygeneCharacter::EndPlayerTurn()
{
	OnPlayerTurnEnded.Broadcast();
}

void ADioxygeneCharacter::SetCombatManager(ACombatManager* Manager)
{
	check(Manager);
	CombatManagerRef = Manager;
}

EParryResult ADioxygeneCharacter::CheckParry(const float PerfectWindow) const
{
	if(bIsParrying)
	{
		if(const float CurrentTime = GetWorld()->GetTimeSeconds(); CurrentTime - ParryStartTime > PerfectWindow)
		{
			return EParryResult::NormalParry;
		}
		return EParryResult::PerfectParry;
	}
	return EParryResult::NoParry;
}

TArray<TSubclassOf<USpellBase>> ADioxygeneCharacter::GetAvailableSpells()
{
	return AvailableSpells;
}

void ADioxygeneCharacter::AddSpell(const TSubclassOf<USpellBase>& NewSpell)
{
	AvailableSpells.AddUnique(NewSpell);
}

void ADioxygeneCharacter::ReceiveDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("DioCharacter : Health after dmg taken : %f"), CurrentHealth);
	if(CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		Die();
	}
}

void ADioxygeneCharacter::Die_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Character died!"))
	bIsDead = true;
}

void ADioxygeneCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADioxygeneCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADioxygeneCharacter::Talk()
{
	UE_LOG(LogTemp, Warning, TEXT("Talk  :  %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
	if (const APlayerStateFfa* MyPlayerState = Cast<APlayerStateFfa>(GetPlayerState()); IsSteamOK)
	{
		// Set the PlayerSteamID
		//PlayerState->InitSteamID();
		UE_LOG(LogTemp, Warning, TEXT("Talk  : steam ID %s"),
			*FString::Printf(TEXT("%llu"), MyPlayerState->GetSteamID().ConvertToUint64()));
		UE_LOG(LogTemp, Warning, TEXT("Talk  : steam ID real %s"),
			*FString::Printf(TEXT("%llu"), SteamUser()->GetSteamID().ConvertToUint64()));
		//CheckOverlappingCharacter();
		for(auto Character : OverlappingCharacters)
		{
			UE_LOG(LogTemp, Warning, TEXT("Talk  : for loop steam id : %s"), *Cast<APlayerStateFfa>(Character->GetPlayerState())->PlayerSteamID);
		}
	}
}

void ADioxygeneCharacter::Interact()
{
	if(const IInterface_Interact* InteractableActor = Cast<IInterface_Interact>(InteractActor))
	{
		InteractableActor->Execute_Interact(InteractActor);
	}
	Server_Interact_Implementation();
}

void ADioxygeneCharacter::StartParry()
{
	bIsParrying = true;
	ParryStartTime = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("Diocharacter : Start parry"));
}

void ADioxygeneCharacter::EndParry()
{
	bIsParrying = false;
	UE_LOG(LogTemp, Warning, TEXT("Diocharacter : End parry"));
}

void ADioxygeneCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (APlayerStateFfa* PS = GetPlayerState<APlayerStateFfa>())
	{
		PS->OnPlayingPhaseChanged.AddUObject(this, &ADioxygeneCharacter::HandlePlayingPhaseChange);
	}
}

void ADioxygeneCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (APlayerStateFfa* PS = GetPlayerState<APlayerStateFfa>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Possessed By"));
		PS->OnPlayingPhaseChanged.AddUObject(this, &ADioxygeneCharacter::HandlePlayingPhaseChange);
	}
}

void ADioxygeneCharacter::HandlePlayingPhaseChange(const APlayerStateFfa* CurrentPlayerState, const EPlayingPhase NewPhase)
{
	if (NewPhase == EPlayingPhase::Combat)
	{
		DisableMovementInput();
		FocusCombatCamera();
	}
	else if(NewPhase == EPlayingPhase::Exploration)
	{
		EnableMovementInput();
	}
}

void ADioxygeneCharacter::DisableMovementInput() const
{
	if(UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
			MoveComp->DisableMovement();
	}
}

void ADioxygeneCharacter::EnableMovementInput() const
{
	if(UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ADioxygeneCharacter::FocusCombatCamera()
{
	//TODO interpole vers une camera fixe, zoom combat, etc..
	UE_LOG(LogTemp, Warning, TEXT("Camera would focus here"));
}

