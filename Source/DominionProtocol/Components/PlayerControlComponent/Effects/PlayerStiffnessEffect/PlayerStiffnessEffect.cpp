// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStiffnessEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"

UPlayerStiffnessEffect::UPlayerStiffnessEffect()
{
	ControlEffectTag = EffectTags::Stiffness;
}

bool UPlayerStiffnessEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerStiffnessEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

bool UPlayerStiffnessEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerStiffnessEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

void UPlayerStiffnessEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
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
