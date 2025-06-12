// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLyingEffect.h"

#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"

UPlayerLyingEffect::UPlayerLyingEffect()
{
	ControlEffectTag = EffectTags::Lying;
}

bool UPlayerLyingEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	return true;
}

bool UPlayerLyingEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	
	return true;
}

void UPlayerLyingEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerLyingEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerLyingEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerLyingEffect::Dash()
{
	// Super::Dash();
}

void UPlayerLyingEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerLyingEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerLyingEffect::Parry()
{
	// Super::Parry();
}

void UPlayerLyingEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerLyingEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerLyingEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerLyingEffect::Interact()
{
	// Super::Interact();
}

void UPlayerLyingEffect::LockOn()
{
	Super::LockOn();
}
