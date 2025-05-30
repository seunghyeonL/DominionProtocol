// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "UObject/Object.h"
#include "PlayerLockOnEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerLockOnEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()
	UPlayerLockOnEffect();

	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;
	virtual void Tick(float DeltaTime) override;
	
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
