// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDummy.h"

#include "DioxygeneCharacter.h"
#include "SWarningOrErrorBox.h"

// Sets default values
AEnemyDummy::AEnemyDummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //turned off!
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnnemyMesh"));
	RootComponent = EnemyMesh;

	CombatTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("CombatTrigger"));
	CombatTrigger->SetupAttachment(RootComponent);
	CombatTrigger->SetSphereRadius(200.f);
	CombatTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemyDummy::OnCombatTriggerOverlap);

}

// Called when the game starts or when spawned
void AEnemyDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// ReSharper disable once CppMemberFunctionMayBeConst (can't because OnComponentBeginOverlap needs a non const delegate)
void AEnemyDummy::OnCombatTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(const ADioxygeneCharacter* Player = Cast<ADioxygeneCharacter>(OtherActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(OtherComp->GetName()));
		if(!OtherComp->IsA(USphereComponent::StaticClass()))
		{
			if(APlayerStateFfa* PS = Cast<APlayerStateFfa>(Player->GetPlayerState()))
			{
				PS->TriggerCombat(this);
			}
		}
	}
}

// Called every frame
void AEnemyDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyDummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

