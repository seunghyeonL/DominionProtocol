// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECategory.h"
#include "FWorldActorData.generated.h"

USTRUCT(BlueprintType)
struct FWorldActorData
{
	GENERATED_BODY()

	FWorldActorData()
		:	Location(FVector::ZeroVector),
			Rotation(FRotator::ZeroRotator),
			bIsActivate(false),
			bIsCollected(false),
			bIsInteracted(false),
			CurrentHealth(0.f)
	{}
	
	UPROPERTY()
	FGuid InstanceGuid;

	UPROPERTY()
	TSubclassOf<AActor> LinkedActorClass;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	bool bIsActivate;

	// 아이템
	UPROPERTY()
	bool bIsCollected;

	// 상호작용 오브젝트
	UPROPERTY()
	bool bIsInteracted;

	// 적
	UPROPERTY()
	float CurrentHealth;
};

USTRUCT(BlueprintType)
struct FRegionActorData
{
	GENERATED_BODY()

	FRegionActorData()
		:	WorldActorArray(TArray<FWorldActorData>()),
			ActorCategory(EWorldActorCategory::None)
	{}
	
	UPROPERTY()
	TArray<FWorldActorData> WorldActorArray;

	UPROPERTY()
	EWorldActorCategory ActorCategory;
};

USTRUCT(BlueprintType)
struct FActivateActorData
{
	GENERATED_BODY()

	UPROPERTY()
	int LocatedRegionIndex;
	
	UPROPERTY()
	AActor* Actor;
};