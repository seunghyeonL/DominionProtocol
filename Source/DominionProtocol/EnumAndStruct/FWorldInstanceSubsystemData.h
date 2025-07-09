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

	FWorldInstanceSubsystemData()
		: IsNewGameDataMap(TMap<FString, bool>()),
		  CurrentLevelName(FString()),
		  CurrentLevelDisplayName(FText::GetEmpty()),
		  RecentCrackName(FText::GetEmpty()),
		  RecentCrackIndex(0),
		  MaxCrackIndex(1),
		  CrackDataMap(TMap<FString, FCrackDataArrayStruct>()),
		  WorldActorDataMap(TMap<FGuid, FWorldActorData>()),
		  bIsDropEssenceExist(false),
		  DropEssenceAmount(0),
		  DropEssenceLocation(FVector::ZeroVector),
		  DropEssenceLocationLevel(FString())
	{
	}

	UPROPERTY()
	TMap<FString, bool> IsNewGameDataMap;

	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FText CurrentLevelDisplayName;

	UPROPERTY()
	FText RecentCrackName;

	UPROPERTY()
	int32 RecentCrackIndex;

	UPROPERTY()
	int32 MaxCrackIndex;

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
