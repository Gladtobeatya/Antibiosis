// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DIOXYGENE_API USpellBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spell")
	FName SpellName = NAME_None;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExecuteSpell(ACharacter* Caster, ACharacter* Target);
	virtual void ExecuteSpell_Implementation(ACharacter* Caster, ACharacter* Target);

};
