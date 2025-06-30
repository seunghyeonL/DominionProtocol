// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util/GameTagList.h"
#include "FWorldActorData.generated.h"

USTRUCT(BlueprintType)
struct FWorldActorData
{
	GENERATED_BODY()

	FWorldActorData()
		:	bIsItemCollected(false),
			bIsChestOpened(false),
			bIsDead(false)
	{}
	
	UPROPERTY()
	FGuid UniqueActorID;

	UPROPERTY()
	FGameplayTag Tag;

	//ItemDropped, OpenableChestItem, ItemContainer
	UPROPERTY()
	bool bIsItemCollected;

	//OpenableChestItem
	UPROPERTY()
	bool bIsChestOpened;

	//Pawn(DyingHelper)
	UPROPERTY()
	bool bIsDead;
};