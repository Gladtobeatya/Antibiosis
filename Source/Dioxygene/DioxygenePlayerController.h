// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DioxygeneGameMode.h"
#include "GameFramework/PlayerController.h"
#include "DioxygenePlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class DIOXYGENE_API ADioxygenePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	virtual void BeginPlay() override;

public :
};
