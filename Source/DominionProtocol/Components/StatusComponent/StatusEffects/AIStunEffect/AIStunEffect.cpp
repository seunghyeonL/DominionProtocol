// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStunEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"

UAIStunEffect::UAIStunEffect()
{
	StatusEffectTag = EffectTags::Stun;
}

bool UAIStunEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIStunEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}
	return true;
}

bool UAIStunEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIStunEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}
	return true;
}

void UAIStunEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}
