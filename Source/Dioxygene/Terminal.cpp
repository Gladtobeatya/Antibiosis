// Fill out your copyright notice in the Description page of Project Settings.


#include "Terminal.h"

// Sets default values
ATerminal::ATerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// Create a mesh component that will be viewed as this terminal
	TerminalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalMesh"));
	TerminalMesh->SetupAttachment(Root);

	//Set default InteractValue, it should match with the other actor(s) this object should trigger when player is interacting
	InteractValue = 0;
}

// Called when the game starts or when spawned
void ATerminal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerminal::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact!"));
}

