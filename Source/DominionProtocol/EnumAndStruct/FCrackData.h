// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCrackData.generated.h"

USTRUCT(BlueprintType)
struct FCrackData
{
	GENERATED_BODY()

	FCrackData()
		:	OwnerLevelName(TEXT("")),
			CrackIndex(0),
			CrackName(FText::GetEmpty()),
			bIsActivate(false),
			RespawnLocation(FVector::ZeroVector),
			RespawnRotation(FRotator::ZeroRotator)
	{}

	UPROPERTY(BlueprintReadOnly)
	FString OwnerLevelName;

	UPROPERTY(BlueprintReadOnly)
	int32 CrackIndex;
	
	UPROPERTY(BlueprintReadOnly)
	FText CrackName;

	UPROPERTY(BlueprintReadOnly)
	bool bIsActivate;
	
	UPROPERTY()
	FVector RespawnLocation;

	UPROPERTY()
	FRotator RespawnRotation;
};

USTRUCT(BlueprintType)
struct FCrackDataArrayStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FCrackData> CrackDataArray;
};