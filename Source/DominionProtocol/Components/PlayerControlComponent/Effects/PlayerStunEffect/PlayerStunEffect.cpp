// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStunEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"

UPlayerStunEffect::UPlayerStunEffect()
{
	ControlEffectTag = EffectTags::Stun;
}

bool UPlayerStunEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerStunEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

bool UPlayerStunEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerStunEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

void UPlayerStunEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
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
