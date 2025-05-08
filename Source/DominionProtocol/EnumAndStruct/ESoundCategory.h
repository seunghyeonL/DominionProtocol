// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ESoundCategory.generated.h"

UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	Master UMETA(DisplayName = "Master"),
	SFX UMETA(DisplayName = "SFX"),
	BGM UMETA(DisplayName = "BGM"),
	UI UMETA(DisplayName = "UI")
};
