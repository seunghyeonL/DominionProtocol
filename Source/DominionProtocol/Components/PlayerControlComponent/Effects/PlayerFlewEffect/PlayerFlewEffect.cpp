// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlewEffect.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"

UPlayerFlewEffect::UPlayerFlewEffect()
{
	ControlEffectTag = EffectTags::Flew;
}

bool UPlayerFlewEffect::Activate()
{
	// return Super::Activate();
	// 날아가는 애니메이션 시간으로 처리
	Activate(2.2f);
	
	return true;
}

bool UPlayerFlewEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerFlewEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}
	
	return true;
}

void UPlayerFlewEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

	// 일어나는 애니메이션 시간만큼 이펙트 실행
	ControlComponent->ActivateControlEffect(EffectTags::Lying, 2.183f);
}

void UPlayerFlewEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerFlewEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerFlewEffect::Dash()
{
	// Super::Dash();
}

void UPlayerFlewEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerFlewEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerFlewEffect::Parry()
{
	// Super::Parry();
}

void UPlayerFlewEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerFlewEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerFlewEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerFlewEffect::Interact()
{
	// Super::Interact();
}

void UPlayerFlewEffect::LockOn()
{
	Super::LockOn();
}
