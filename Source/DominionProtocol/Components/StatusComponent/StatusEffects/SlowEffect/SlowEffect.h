// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"
#include "SlowEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API USlowEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	USlowEffect();
	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;
	
};
