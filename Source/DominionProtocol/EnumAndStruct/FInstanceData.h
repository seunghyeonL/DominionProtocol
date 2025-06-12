// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util/GameTagList.h"
#include "FInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FInstanceData
{
	GENERATED_BODY()

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	int32 SaveSlotIndex;
	
	UPROPERTY()
	TMap<FGameplayTag, float> StatDataMap;

	UPROPERTY()
	int32 PlayerCurrentEssence;

	UPROPERTY()
	int32 PlayTime;
};
