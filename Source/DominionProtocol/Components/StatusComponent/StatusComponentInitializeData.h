#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StatusComponentInitializeData.generated.h"

USTRUCT(BlueprintType)
struct FTagFloatPair 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};

USTRUCT(BlueprintType)
struct FTagEffectClassPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UStatusEffectBase> EffectClass;
};

USTRUCT(BlueprintType)
struct FStatusComponentInitializeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTagFloatPair> StatDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTagFloatPair> StatMultiplierDatas; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTagEffectClassPair> EffectClassDatas;
};
