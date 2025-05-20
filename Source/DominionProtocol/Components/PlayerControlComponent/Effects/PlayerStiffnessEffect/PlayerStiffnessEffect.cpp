// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStiffnessEffect.h"

UPlayerStiffnessEffect::UPlayerStiffnessEffect()
{
	ControlEffectTag = EffectTags::Stiffness;
}

void UPlayerStiffnessEffect::Activate()
{
	Super::Activate();
}

void UPlayerStiffnessEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerStiffnessEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerStiffnessEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerStiffnessEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerStiffnessEffect::Dash()
{
	// Super::Dash();
}

void UPlayerStiffnessEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerStiffnessEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerStiffnessEffect::Parry()
{
	// Super::Parry();
}

void UPlayerStiffnessEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerStiffnessEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerStiffnessEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerStiffnessEffect::Interact()
{
	// Super::Interact();
}

void UPlayerStiffnessEffect::LockOn()
{
	// Super::LockOn();
}
