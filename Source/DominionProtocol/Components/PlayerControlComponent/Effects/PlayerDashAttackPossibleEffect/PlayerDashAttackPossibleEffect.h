// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "PlayerDashAttackPossibleEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerDashAttackPossibleEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()

public:
	UPlayerDashAttackPossibleEffect();

	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;

	virtual void BaseAttack() override;
};
