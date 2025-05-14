// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashSkill.h"

#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/GameTagList.h"

UPlayerDashSkill::UPlayerDashSkill()
{
	SkillTag = SkillTags::PlayerDash;
	ControlEffectTag = EffectTags::UsingDash;
}

void UPlayerDashSkill::Execute(ACharacter* Owner)
{
	// Super::Execute(Owner);
	auto ControlComponentUser = Cast<IControlComponentUser>(GetOuter());
	check(ControlComponentUser)

	auto ControlComponent = Cast<UPlayerControlComponent>(ControlComponentUser->GetPlayerControlComponent());
	check(ControlComponent)

	ControlComponent->ActivateControlEffect(EffectTags::UsingDash, DashDuration);
	
}

void UPlayerDashSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void UPlayerDashSkill::StartDash()
{
	

	
	
	
}
