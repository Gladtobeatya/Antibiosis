// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EParryResult : uint8
{
	NoParry UMETA(DisplayName="NoParry"),
	NormalParry UMETA(DisplayName="NormalParry"),
	PerfectParry UMETA(DisplayName="PerfectParry"),
};