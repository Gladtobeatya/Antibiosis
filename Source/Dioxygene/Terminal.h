// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface_Interact.h"
#include "GameFramework/Actor.h"
#include "Terminal.generated.h"

UCLASS()
class DIOXYGENE_API ATerminal : public AActor, public IInterface_Interact
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	/** Actor mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TerminalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Interaction", meta = (AllowPrivateAccess = "true"))
	int InteractValue;
	
	
public:	
	// Sets default values for this actor's properties
	ATerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation() override;
};
