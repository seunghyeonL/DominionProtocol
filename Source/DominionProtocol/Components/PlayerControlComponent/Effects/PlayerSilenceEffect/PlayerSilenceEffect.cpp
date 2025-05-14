// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSilenceEffect.h"

UPlayerSilenceEffect::UPlayerSilenceEffect()
{
	ControlEffectTag = EffectTags::Silence;
}

void UPlayerSilenceEffect::Activate()
{
	Super::Activate();
}

void UPlayerSilenceEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerSilenceEffect::Deactivate()
{
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

void UPlayerSilenceEffect::Sprint()
{
	Super::Sprint();
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
