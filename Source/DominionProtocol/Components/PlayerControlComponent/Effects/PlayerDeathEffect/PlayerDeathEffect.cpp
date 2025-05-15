// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDeathEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UPlayerDeathEffect::UPlayerDeathEffect()
{
	ControlEffectTag = EffectTags::Death;
}

void UPlayerDeathEffect::Activate()
{
	Super::Activate();

	// Ignore Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void UPlayerDeathEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerDeathEffect::Deactivate()
{
	Super::Deactivate();

	// Ignore Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
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

void UPlayerDeathEffect::SprintStart()
{
	// Super::SprintStart();
}

void UPlayerDeathEffect::SprintEnd()
{
	// Super::SprintEnd();
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

void UPlayerDeathEffect::Interact()
{
	// Super::Interact();
}

void UPlayerDeathEffect::LockOn()
{
	// Super::LockOn();
}
