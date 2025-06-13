// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStandingUpEffect.h"

#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"

UPlayerStandingUpEffect::UPlayerStandingUpEffect()
{
	ControlEffectTag = EffectTags::StandingUp;
}

bool UPlayerStandingUpEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	return true;
}

bool UPlayerStandingUpEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	
	return true;
}

void UPlayerStandingUpEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerStandingUpEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerStandingUpEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerStandingUpEffect::Dash()
{
	// Super::Dash();
}

void UPlayerStandingUpEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerStandingUpEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerStandingUpEffect::Parry()
{
	// Super::Parry();
}

void UPlayerStandingUpEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerStandingUpEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerStandingUpEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerStandingUpEffect::Interact()
{
	// Super::Interact();
}

void UPlayerStandingUpEffect::LockOn()
{
	Super::LockOn();
}
