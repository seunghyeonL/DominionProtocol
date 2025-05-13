// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashSkill.h"

#include "Util/GameTagList.h"

UPlayerDashSkill::UPlayerDashSkill()
{
	SkillTag = SkillTags::PlayerDash;
	ControlEffectTag = EffectTags::UsingDash;
}

void UPlayerDashSkill::Execute(ACharacter* Owner)
{
	// Super::Execute(Owner);
	
	
}
