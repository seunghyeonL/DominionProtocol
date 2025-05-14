// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashSkill.h"

#include "Components/PlayerControlComponent/ControlComponentUser.h"
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
}

FVector UPlayerDashSkill::GetDashDirection() const
{
	check(OwnerCharacter);
	if (auto ControlComponentUser = Cast<IControlComponentUser>(OwnerCharacter))
	{
		auto LastMovementVector = ControlComponentUser->GetLastMovementVector();
		
		if (LastMovementVector.IsNearlyZero())
		{
			return -OwnerCharacter->GetActorForwardVector();
		}
		
		return LastMovementVector.GetSafeNormal();
	}

	Debug::PrintError("UPlayerDashSkill::GetDashDirection : OwnerCharacter doesn't implement ControlComponentUser.");
	return {0.f, 0.f, 0.f};
}

void UPlayerDashSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	check(OwnerCharacter);
	
	FVector Step = GetDashDirection() * DashSpeed * DeltaTime;
	
	FHitResult Hit;
	OwnerCharacter->GetCharacterMovement()->SafeMoveUpdatedComponent(
		Step,
		OwnerCharacter->GetActorRotation(),
		true,
		Hit
	);
}
