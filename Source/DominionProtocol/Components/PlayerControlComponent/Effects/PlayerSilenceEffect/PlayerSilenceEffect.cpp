// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSilenceEffect.h"

UPlayerSilenceEffect::UPlayerSilenceEffect()
{
	ControlEffectTag = EffectTags::Silence;
}

bool UPlayerSilenceEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	return true;
}

bool UPlayerSilenceEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	return true;
}

void UPlayerSilenceEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}

void UPlayerSilenceEffect::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
}

void UPlayerSilenceEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerSilenceEffect::Dash()
{
	// Super::Dash();
}

void UPlayerSilenceEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerSilenceEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerSilenceEffect::Parry()
{
	// Super::Parry();
}

void UPlayerSilenceEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerSilenceEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerSilenceEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerSilenceEffect::Interact()
{
	Super::Interact();
}

void UPlayerSilenceEffect::LockOn()
{
	Super::LockOn();
}
