// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util/GameTagList.h"
#include "FItemSubsystemData.generated.h"

USTRUCT(BlueprintType)
struct FItemSubsystemData
{
	GENERATED_BODY()

	FItemSubsystemData()
		:	bIsPotionBoostApplied(false)
	{}
	
	UPROPERTY()
	TMap<FGameplayTag, int32> InventoryMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FGameplayTag> EquipmentSlots;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Consumable")
	TMap<FName, FGameplayTag> ConsumableSlots;

	UPROPERTY()
	bool bIsPotionBoostApplied;
};
