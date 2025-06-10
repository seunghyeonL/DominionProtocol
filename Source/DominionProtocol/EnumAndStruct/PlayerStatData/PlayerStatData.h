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
	float Weapon1AttackPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weapon2AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaRecoveryRate; 
};
