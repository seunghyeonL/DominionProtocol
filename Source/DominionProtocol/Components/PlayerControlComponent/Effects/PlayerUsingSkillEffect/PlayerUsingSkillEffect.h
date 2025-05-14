// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "PlayerUsingSkillEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerUsingSkillEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()

public:
	UPlayerUsingSkillEffect();

	FORCEINLINE void SetControlEffectTag(FGameplayTag InControlEffectTag) { ControlEffectTag = InControlEffectTag; }
	
	virtual void Activate() override;
	virtual void Activate(float Duration) override;
	virtual void Deactivate() override;
	
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Dash() override;
	virtual void Sprint() override;
	virtual void Parry() override;
	virtual void BaseAttack() override;
	virtual void WeaponSkill() override;
	virtual void MagicSkill() override;
	virtual void Interact() override;
	virtual void LockOn() override;

	virtual void Tick(float DeltaTime) override;
};
