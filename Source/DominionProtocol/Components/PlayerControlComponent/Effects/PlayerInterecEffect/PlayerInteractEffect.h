// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "PlayerInteractEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerInteractEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()

public:
	UPlayerInteractEffect();
	
	virtual bool Activate();
	virtual bool Activate(float Duration);
	virtual void Deactivate();

	// Input Binding Functions
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

	virtual void ConsumeItemAction_1()override;
	virtual void ConsumeItemAction_2()override;
	virtual void ConsumeItemAction_3()override;
	virtual void SwapWeapon()override;

protected:
	float InteractMontageDuration;	
};
