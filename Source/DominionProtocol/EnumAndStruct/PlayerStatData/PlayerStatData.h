#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlayerStatData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Str;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float End;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PrimaryAttackPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SubAttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float StaminaRecoveryRate; 
};
