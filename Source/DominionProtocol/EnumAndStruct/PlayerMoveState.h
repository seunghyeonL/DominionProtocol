// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMoveState.generated.h"

UENUM(BlueprintType)
enum class EPlayerMoveState : uint8
{
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	MAX UMETA(DisplayName = "MAX")
};
