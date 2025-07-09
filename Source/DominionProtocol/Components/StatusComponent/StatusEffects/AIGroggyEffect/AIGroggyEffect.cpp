// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGroggyEffect.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/StatusComponent/StatusComponent.h"

UAIGroggyEffect::UAIGroggyEffect()
{
	StatusEffectTag = EffectTags::Groggy;
}

bool UAIGroggyEffect::Activate()
{
	if (!Super::Activate()) return false;

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIGroggyEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

bool UAIGroggyEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration)) return false;

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UAIGroggyEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

void UAIGroggyEffect::Deactivate()
{
	Super::Deactivate();

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	check(StatusComponent);

	StatusComponent->SetStat(StatTags::GroggyGauge, StatusComponent->GetStat(StatTags::MaxGroggyGauge));
}
