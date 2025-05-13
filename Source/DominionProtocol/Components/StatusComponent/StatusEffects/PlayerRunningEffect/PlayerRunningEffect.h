// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"
#include "PlayerRunningEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerRunningEffect : public UStatusEffectBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coefficient")
	float SpeedCoefficient;

public:
	UPlayerRunningEffect();

	virtual void Activate(float Magnitude) override;
	virtual void Activate(float Magnitude, float Duration) override;
	virtual void Deactivate() override;
};
