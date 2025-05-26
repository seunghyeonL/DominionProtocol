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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OwnerLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LinkedLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CrackIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CrackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RespawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RespawnRotation;
	
};

USTRUCT(BlueprintType)
struct FCrackDataArrayStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FCrackData> CrackDataArray;
};