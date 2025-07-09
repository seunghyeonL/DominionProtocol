// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../StatusEffectBase.h"
#include "PoisonEffect.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UPoisonEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	UPoisonEffect();

	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;

	UFUNCTION()
	void ApplyDoTDamage();
};
