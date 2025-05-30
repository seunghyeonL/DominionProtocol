// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStiffnessEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"

UAIStiffnessEffect::UAIStiffnessEffect()
{
	StatusEffectTag = EffectTags::Stiffness;
	bIsActive = false;
}

bool UAIStiffnessEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIStiffnessEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}
	return true;
}

bool UAIStiffnessEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIStiffnessEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

void UAIStiffnessEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}
