#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BattleDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude { 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration { 0.f };
};

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage { 0.f };
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LaunchVector { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectData> Effects;
};
