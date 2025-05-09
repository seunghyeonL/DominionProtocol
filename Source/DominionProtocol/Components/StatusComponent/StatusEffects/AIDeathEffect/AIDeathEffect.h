// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"
#include "AIDeathEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAIDeathEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	UAIDeathEffect();

	virtual void Activate() override;
	virtual void Activate(float Duration) override;
	virtual void Deactivate() override;
};
