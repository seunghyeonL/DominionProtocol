// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECategory.generated.h"

UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	Master UMETA(DisplayName = "Master"),
	SFX UMETA(DisplayName = "SFX"),
	BGM UMETA(DisplayName = "BGM"),
	UI UMETA(DisplayName = "UI")
};

UENUM(BlueprintType)
enum class EWorldActorCategory : uint8
{
	None UMETA(DisplayName = "None"),
	Item UMETA(DisplayName = "Item"),
	Interactable UMETA(DisplayName = "Interactable"),
	Enemy UMETA(DisplayName = "Enemy")
};