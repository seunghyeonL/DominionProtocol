// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FInstanceData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<bool> IsBossDeadArray;
};
