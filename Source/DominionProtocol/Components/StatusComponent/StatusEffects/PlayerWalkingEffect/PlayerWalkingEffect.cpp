// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWalkingEffect.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerWalkingEffect::UPlayerWalkingEffect()
{
	StatusEffectTag = EffectTags::Walking;
	SpeedCoefficient = 0.5f;
}

bool UPlayerWalkingEffect::Activate()
{
	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	check(StatusComponent);
	
	auto ActiveStatusEffectTags = StatusComponent->GetActiveStatusEffectTags();
	if (ActiveStatusEffectTags.HasTag(EffectTags::Running))
	{
		Super::Deactivate();
		return false;
	}
	
	if (!Super::Activate())
	{
		return false;
	}
	
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerWalkingEffect::Activate : Invalid OwnerCharacter."));
		return false;
	}

	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	MovementComponent->MaxWalkSpeed *= SpeedCoefficient;
	return true;
}

bool UPlayerWalkingEffect::Activate(float Duration)
{
	Debug::PrintError(TEXT("UPlayerWalkingEffect::Activate : Use Activate()!"));
	Activate();
	return true;
}

void UPlayerWalkingEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
	
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerWalkingEffect::Deactivate : Invalid OwnerCharacter."));
		return;
	}

	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	MovementComponent->MaxWalkSpeed /= SpeedCoefficient;
}
