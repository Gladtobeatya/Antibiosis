// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Interact : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIOXYGENE_API IInterface_Interact
{
	GENERATED_BODY()

public:

	//BlueprintNativeEvent = default implementation in c++, and can be overriden in blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	//BlueprintImplementableEvent = only blueprint can implement it
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void SetFocused();
};
