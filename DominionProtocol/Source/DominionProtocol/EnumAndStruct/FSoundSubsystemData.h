// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/ESoundCategory.h"
#include "FSoundSubsystemData.generated.h"

USTRUCT(BlueprintType)
struct FSoundSubsystemData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ESoundCategory, float> CategoryVolumes;
};
