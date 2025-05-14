// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/PlayerSkill/PlayerParrySkill/PlayerParrySkill.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/GameTagList.h"

UPlayerParrySkill::UPlayerParrySkill()
{
	SkillTag = SkillTags::PlayerParry;
	ControlEffectTag = EffectTags::UsingParry;
}

void UPlayerParrySkill::Execute()
{
	auto ControlComponentUser = Cast<IControlComponentUser>(GetOuter());
	auto ControlComponent = ControlComponentUser->GetPlayerControlComponent();

	ControlComponent->ActivateControlEffect(EffectTags::UsingParry, ParryDuration);
}