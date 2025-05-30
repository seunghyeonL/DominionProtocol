// Fill out your copyright notice in the Description page of Project Settings.


#include "AIParriedEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"

UAIParriedEffect::UAIParriedEffect()
{
	StatusEffectTag = EffectTags::Parried;
	bIsActive = false;
}

bool UAIParriedEffect::Activate()
{
	if (!Super::Activate()) return false;

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIParriedEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
	
}

bool UAIParriedEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration)) return false;

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIParriedEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

void UAIParriedEffect::Deactivate()
{
	Super::Deactivate();
}
