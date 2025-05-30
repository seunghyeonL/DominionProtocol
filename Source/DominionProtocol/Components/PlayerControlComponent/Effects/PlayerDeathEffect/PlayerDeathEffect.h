﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerControlEffectBase.h"
#include "PlayerDeathEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerDeathEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()
	
public:
	UPlayerDeathEffect();
	
	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;
	
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
};
