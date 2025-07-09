// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerConsumingItemEffect.h"

UPlayerConsumingItemEffect::UPlayerConsumingItemEffect()
{
	ControlEffectTag = EffectTags::ConsumingItem;
}

void UPlayerConsumingItemEffect::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
}

void UPlayerConsumingItemEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerConsumingItemEffect::Dash()
{
	// Super::Dash();
}

void UPlayerConsumingItemEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerConsumingItemEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerConsumingItemEffect::Parry()
{
	// Super::Parry();
}

void UPlayerConsumingItemEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerConsumingItemEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerConsumingItemEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerConsumingItemEffect::Interact()
{
	// Super::Interact();
}

void UPlayerConsumingItemEffect::LockOn()
{
	Super::LockOn();
}

void UPlayerConsumingItemEffect::ConsumeItemAction_1()
{
	// Super::ConsumeItemAction_1();
}

void UPlayerConsumingItemEffect::ConsumeItemAction_2()
{
	// Super::ConsumeItemAction_2();
}

void UPlayerConsumingItemEffect::ConsumeItemAction_3()
{
	// Super::ConsumeItemAction_3();
}
