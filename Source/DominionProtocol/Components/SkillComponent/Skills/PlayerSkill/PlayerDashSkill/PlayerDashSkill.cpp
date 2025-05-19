// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashSkill.h"

#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"
#include "Util/GameTagList.h"

UPlayerDashSkill::UPlayerDashSkill()
{
	SkillTag = SkillTags::PlayerDash; 
	ControlEffectTag = EffectTags::UsingDash;
	DashMoveDirection = { 0.f, 0.f, 0.f };
	DashSpeed = 1000.f;
}

void UPlayerDashSkill::Execute()
{
	Super::Execute();
	SetDashDirection();
}

void UPlayerDashSkill::SetDashDirection()
{
	check(IsValid(OwnerCharacter));
	
	auto ControlComponent = OwnerCharacter->FindComponentByClass<UPlayerControlComponent>();
	check(IsValid(ControlComponent));

	auto LastMovementVector = ControlComponent->GetCurrentMovementVector();
		
	if (LastMovementVector.IsNearlyZero())
	{
		DashMoveDirection = -OwnerCharacter->GetActorForwardVector();
	}
	else
	{
		DashMoveDirection = LastMovementVector.GetSafeNormal();
	}
}

void UPlayerDashSkill::Tick(float DeltaTime)
{
	check(OwnerCharacter);
	
	FVector Step = DashMoveDirection * DashSpeed * DeltaTime;
	
	FHitResult Hit;
	OwnerCharacter->GetCharacterMovement()->SafeMoveUpdatedComponent(
		Step,
		OwnerCharacter->GetActorRotation(),
		true,
		Hit
	);
}
