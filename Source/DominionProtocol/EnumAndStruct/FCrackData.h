// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCrackData.generated.h"

USTRUCT(BlueprintType)
struct FCrackData
{
	GENERATED_BODY()

	FCrackData()
		:	bIsActivate(false),
			RespawnLocation(FVector::ZeroVector),
			RespawnRotation(FRotator::ZeroRotator)
	{}

	UPROPERTY()
	FString OwnerLevelName;

	UPROPERTY()
	int32 CrackIndex;
	
	UPROPERTY()
	FText CrackName;

	UPROPERTY()
	bool bIsActivate;
	
	UPROPERTY()
	FVector RespawnLocation;

	UPROPERTY()
	FRotator RespawnRotation;
};

USTRUCT(BlueprintType)
struct FCrackDataArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FCrackData> CrackDataArray;
};