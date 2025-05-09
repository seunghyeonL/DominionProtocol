// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StatusEffectBase.h"
#include "AttackDownEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAttackDownEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	UAttackDownEffect();

	virtual void Activate(float Magnitude) override;
	virtual void Activate(float Magnitude, float Duration) override;
	virtual void Deactivate() override;
};
