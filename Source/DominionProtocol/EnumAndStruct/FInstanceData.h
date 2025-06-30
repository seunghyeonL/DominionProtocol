// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Util/GameTagList.h"
#include "FInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FInstanceData
{
	GENERATED_BODY()

	FInstanceData()
		: SaveSlotName(FString()),
		  SaveSlotIndex(-1),
		  StatDataMap(TMap<FGameplayTag, float>()),
		  CurrentGameStoryState(EGameStoryState::Tutorial),
		  PlayTime(0),
		  PlayerCurrentEssence(0)
	{
	}

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	int32 SaveSlotIndex;

	UPROPERTY()
	TMap<FGameplayTag, float> StatDataMap;

	UPROPERTY()
	EGameStoryState CurrentGameStoryState;

	UPROPERTY()
	int32 PlayTime;

	UPROPERTY()
	int32 PlayerCurrentEssence;
};
