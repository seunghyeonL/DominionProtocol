// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDeathEffect.h"

UPlayerDeathEffect::UPlayerDeathEffect()
{
	ControlEffectTag = EffectTags::Death;
	bIsActive = false;
	OuterState = nullptr;
	InnerState = nullptr;
}

void UPlayerDeathEffect::Activate()
{
	Super::Activate();
}

void UPlayerDeathEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerDeathEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerDeathEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerDeathEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerDeathEffect::Dash()
{
	// Super::Dash();
}

void UPlayerDeathEffect::Sprint()
{
	// Super::Sprint();
}

void UPlayerDeathEffect::Parry()
{
	// Super::Parry();
}

void UPlayerDeathEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerDeathEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerDeathEffect::MagicSkill()
{
	// Super::MagicSkill();
}
