// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStunEffect.h"

UPlayerStunEffect::UPlayerStunEffect()
{
	ControlEffectTag = EffectTags::Stun;
}

void UPlayerStunEffect::Activate()
{
	Super::Activate();
}

void UPlayerStunEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerStunEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerStunEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerStunEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerStunEffect::Dash()
{
	// Super::Dash();
}

void UPlayerStunEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerStunEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerStunEffect::Parry()
{
	// Super::Parry();
}

void UPlayerStunEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerStunEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerStunEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerStunEffect::Interact()
{
	// Super::Interact();
}

void UPlayerStunEffect::LockOn()
{
	// Super::LockOn();
}
