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

	//Needed for rotation and location
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	/** Actor mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TerminalMesh;
	
public:	
	// Sets default values for this actor's properties
	ATerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Links")
	TArray<TObjectPtr<AActor>> LinkedActors;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void CallLinkedActors();
	
	
};
