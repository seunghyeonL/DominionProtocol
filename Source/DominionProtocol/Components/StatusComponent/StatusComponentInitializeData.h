#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StatusComponentInitializeData.generated.h"

USTRUCT(BlueprintType)
struct FTagFloatPair 
{
	GENERATED_BODY()

	FTagFloatPair() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;
};

USTRUCT(BlueprintType)
struct FTagEffectClassPair
{
	GENERATED_BODY()

	FTagEffectClassPair() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UStatusEffectBase> EffectClass;
};

USTRUCT(BlueprintType)
struct FStatusComponentInitializeData : public FTableRowBase
{
	GENERATED_BODY()

	FStatusComponentInitializeData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTagFloatPair> StatDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTagEffectClassPair> EffectClassDatas;
};
