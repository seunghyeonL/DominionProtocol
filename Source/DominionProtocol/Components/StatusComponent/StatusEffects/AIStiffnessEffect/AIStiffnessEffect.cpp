// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStiffnessEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"

UAIStiffnessEffect::UAIStiffnessEffect()
{
	StatusEffectTag = EffectTags::Stiffness;
	bIsActive = false;
}

void UAIStiffnessEffect::Activate()
{
	Super::Activate();
	
	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIStiffnessEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}
}

void UAIStiffnessEffect::Activate(float Duration)
{
	Super::Activate(Duration);

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIStiffnessEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}
}

void UAIStiffnessEffect::Deactivate()
{
	Super::Deactivate();
}
