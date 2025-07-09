#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EffectInitializeData.generated.h"


USTRUCT(BlueprintType)
struct FEffectInitializeData : public FTableRowBase
{
	GENERATED_BODY()

	FEffectInitializeData()
		: EffectTag(FGameplayTag()),
		  EffectIcon(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EffectIcon;
};
