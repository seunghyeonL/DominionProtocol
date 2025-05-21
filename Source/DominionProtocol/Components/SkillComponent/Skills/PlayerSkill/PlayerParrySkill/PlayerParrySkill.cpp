// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/PlayerSkill/PlayerParrySkill/PlayerParrySkill.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "GameFramework/Character.h"
#include "Util/DebugHelper.h"
#include "Util/GameTagList.h"

UPlayerParrySkill::UPlayerParrySkill()
{
	SkillTag = SkillTags::PlayerParry;
	ControlEffectTag = EffectTags::UsingParry;
}

void UPlayerParrySkill::Execute()
{
	check(IsValid(OwnerCharacter));
	auto ControlComponent = OwnerCharacter->FindComponentByClass<UPlayerControlComponent>();
	check(IsValid(ControlComponent));

	Super::Execute();
	ControlComponent->ActivateControlEffect(EffectTags::UsingParry);
}