#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlayerStatData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStatData
{
	GENERATED_BODY()

	FPlayerStatData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Str = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spl = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float End = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PrimaryAttackPower = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SubAttackPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 0.f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float StaminaRecoveryRate; 
};
