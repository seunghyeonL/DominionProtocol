// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUsingDashEffect.h"

void UPlayerUsingDashEffect::Activate()
{
	Super::Activate();

	ControlEffectTag = EffectTags::UsingDash;
	bIsActive = false;
	OuterState = nullptr;
	InnerState = nullptr;
}

void UPlayerUsingDashEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerUsingDashEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerUsingDashEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerUsingDashEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerUsingDashEffect::Dash()
{
	// Super::Dash();
}

void UPlayerUsingDashEffect::Sprint()
{
	// Super::Sprint();
}

void UPlayerUsingDashEffect::Parry()
{
	// Super::Parry();
}

void UPlayerUsingDashEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerUsingDashEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerUsingDashEffect::MagicSkill()
{
	// Super::MagicSkill();
}
