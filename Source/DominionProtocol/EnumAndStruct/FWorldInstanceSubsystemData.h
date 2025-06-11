// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWorldActorData.h"
#include "FCrackData.h"
#include "WorldObjects/DropEssence.h"
#include "FWorldInstanceSubsystemData.generated.h"

USTRUCT(BlueprintType)
struct FWorldInstanceSubsystemData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FString, bool> IsNewGameDataMap;
	
	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FText CurrentLevelDisplayName;

	UPROPERTY()
	FText RecentCrackName;

	UPROPERTY()
	int32 RecentCrackIndex = 0;

	UPROPERTY()
	int32 MaxCrackIndex = 1;

	UPROPERTY()
	TMap<FString, FCrackDataArrayStruct> CrackDataMap;

	UPROPERTY()
	TMap<FGuid, FWorldActorData> WorldActorDataMap;
	
	UPROPERTY()
	bool bIsDropEssenceExist;
	
	UPROPERTY()
	int32 DropEssenceAmount;

	UPROPERTY()
	FVector DropEssenceLocation;

	UPROPERTY()
	FString DropEssenceLocationLevel;
};
