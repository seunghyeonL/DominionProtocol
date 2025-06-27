#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EffectUIData.generated.h"

USTRUCT(BlueprintType)
struct FEffectUIData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EffectName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EffectIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DurationRemained = 0.f;
};
