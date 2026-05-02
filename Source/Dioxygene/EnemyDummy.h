// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "EnemyDummy.generated.h"

class ACombatEncounter;

UCLASS()
class DIOXYGENE_API AEnemyDummy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* EnemyMesh;

	//Triggers the start of the combat
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CombatTrigger;

	//Handles basic behavior on trigger overlap
	UFUNCTION()
	void OnCombatTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Linked to a parent encounter so it spawns enemies etc
	UPROPERTY(EditInstanceOnly, Category="Combat")
	ACombatEncounter* ParentEncounter;

};
