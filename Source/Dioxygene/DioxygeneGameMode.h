// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DioxygeneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DIOXYGENE_API AGameModeFFA : public AGameMode
{
	GENERATED_BODY()

	

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Get transform of a random player start*/
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	FTransform FindRandomPlayerStart();
};



