// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDeathEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"

UPlayerDeathEffect::UPlayerDeathEffect()
{
	ControlEffectTag = EffectTags::Death;
}

bool UPlayerDeathEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerDeathEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	// Ignore Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}

	return true;
}

bool UPlayerDeathEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		Debug::Print(TEXT("UPlayerDeathEffect::Activate : StopSkill."));
		SkillComponent->StopSkill();
	}

	return true;
}

void UPlayerDeathEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
	
	// Set Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

	// 일어나는 애니메이션 시간만큼 이펙트 실행
	ControlComponent->ActivateControlEffect(EffectTags::Lying, 2.183f);
}

void UPlayerDeathEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerDeathEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerDeathEffect::Dash()
{
	// Super::Dash();
}

void UPlayerDeathEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerDeathEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerDeathEffect::Parry()
{
	// Super::Parry();
}

void UPlayerDeathEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerDeathEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerDeathEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerDeathEffect::Interact()
{
	// Super::Interact();
}

void UPlayerDeathEffect::LockOn()
{
	Super::LockOn();
}
