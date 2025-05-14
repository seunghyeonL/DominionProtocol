// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUsingSkillEffect.h"
#include "Util/DebugHelper.h"

UPlayerUsingSkillEffect::UPlayerUsingSkillEffect()
{
	ControlEffectTag = EffectTags::UsingSkill;
}

void UPlayerUsingSkillEffect::Activate()
{
	Super::Activate();
}

void UPlayerUsingSkillEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerUsingSkillEffect::Deactivate()
{
	Super::Deactivate();
	SetControlEffectTag(EffectTags::UsingSkill);
}

void UPlayerUsingSkillEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerUsingSkillEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerUsingSkillEffect::Dash()
{
	// Super::Dash();
}

void UPlayerUsingSkillEffect::Sprint()
{
	Super::Sprint();
}

void UPlayerUsingSkillEffect::Parry()
{
	// Super::Parry();
}

void UPlayerUsingSkillEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerUsingSkillEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerUsingSkillEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerUsingSkillEffect::Interact()
{
	// Super::Interact();
}

void UPlayerUsingSkillEffect::LockOn()
{
	// Super::LockOn();
}
