// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "FCrackImageData.generated.h"

USTRUCT(BlueprintType)
struct FCrackImageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CrackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrackImageData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CrackDescription;

	FCrackImageData()
		: CrackIndex(-1),
		  CrackImageData(nullptr),
		  CrackDescription(FText::GetEmpty())
	{
	}
};
