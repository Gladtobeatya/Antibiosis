// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to NOT call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(Root);

	IsOpen = false;
	
	// Enable replication (DO NOT TOUCH)
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnRep_IsOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep call!"));
	if(IsOpen)
	{
		Open();
	}
	else
	{
		Close();
	}
}

void ADoor::ToggleDoor_Implementation()
{
	IsOpen = !IsOpen;
	OnRep_IsOpen();
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate IsOpen variable
	DOREPLIFETIME(ADoor, IsOpen);
}

void ADoor::Interact_Implementation()
{
	if (!HasAuthority())
	{
		ToggleDoor_Implementation();
	}
	else
	{
		ToggleDoor();
	}
}

