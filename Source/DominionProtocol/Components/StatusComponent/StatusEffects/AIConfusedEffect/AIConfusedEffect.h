// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StatusEffectBase.h"
#include "AIConfusedEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAIConfusedEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	UAIConfusedEffect();

	virtual void Activate() override;
	virtual void Activate(float Duration) override;
	virtual void Deactivate() override;
};
