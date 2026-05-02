// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface_Interact.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class DIOXYGENE_API ADoor : public AActor, public IInterface_Interact
{
	GENERATED_BODY()
	
	//Needed for rotation and location
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void Open();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void Close();

	//Rep notify (players who join after will still be in sync)
	UPROPERTY(ReplicatedUsing=OnRep_IsOpen)
	bool IsOpen;

	UFUNCTION()
	void OnRep_IsOpen();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ToggleDoor();
	void ToggleDoor_Implementation();

	// Setup replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Interact_Implementation() override;
};
