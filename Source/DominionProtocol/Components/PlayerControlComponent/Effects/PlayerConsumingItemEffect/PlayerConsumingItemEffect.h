﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "PlayerConsumingItemEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerConsumingItemEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()

public:
	UPlayerConsumingItemEffect();
	
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Dash() override;
	virtual void SprintStart() override;
	virtual void SprintEnd() override;
	virtual void Parry() override;
	virtual void BaseAttack() override;
	virtual void WeaponSkill() override;
	virtual void MagicSkill() override;
	virtual void Interact() override;
	virtual void LockOn() override;
	virtual void ConsumeItemAction_1() override;
	virtual void ConsumeItemAction_2() override;
	virtual void ConsumeItemAction_3() override;
};
