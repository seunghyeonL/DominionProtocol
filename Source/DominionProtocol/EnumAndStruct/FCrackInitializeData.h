// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/FCrackData.h"
#include "FCrackInitializeData.generated.h"

USTRUCT(BlueprintType)
struct FCrackInitializeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCrackData ZeroIndexCrackData;
};
