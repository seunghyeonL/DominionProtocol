// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGameStoryState.generated.h"

UENUM(BlueprintType)
enum class EGameStoryState : uint8
{
	Tutorial = 0 UMETA(DisplayName = "튜토리얼"),
	OperateLever = 1 UMETA(DisplayName = "레버 활성화"),
	BattleWithBoss = 2 UMETA(DisplayName = "보스와 전투"),
	ObtainKey = 3 UMETA(DisplayName = "열쇠 습득"),
	UnlockBlockedPath = 4 UMETA(DisplayName = "막힌 길 해금"),
};