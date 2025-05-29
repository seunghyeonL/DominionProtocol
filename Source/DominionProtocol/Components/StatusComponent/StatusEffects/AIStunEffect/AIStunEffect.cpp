// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStunEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"

UAIStunEffect::UAIStunEffect()
{
	StatusEffectTag = EffectTags::Stun;
}

void UAIStunEffect::Activate()
{
	Super::Activate();
	if (bIsActive)
	{
		return;
	}
	bIsActive = true;

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		SkillComponent->StopSkill();
	}
}

void UAIStunEffect::Activate(float Duration)
{
	Super::Activate(Duration);
	if (bIsActive)
	{
		return;
	}
	bIsActive = true;

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		SkillComponent->StopSkill();
	}
}

void UAIStunEffect::Deactivate()
{
	Super::Deactivate();
	if (!bIsActive)
	{
		return;
	}
	bIsActive = false;
}
