// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowEffect.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USlowEffect::USlowEffect()
{
	StatusEffectTag = EffectTags::Slow;
}

bool USlowEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	check(OwnerCharacter);

	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	MovementComponent->MaxWalkSpeed /= Magnitude;
	return true;
}

bool USlowEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	
	check(OwnerCharacter);

	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	MovementComponent->MaxWalkSpeed /= Magnitude;
	return true;
}

void USlowEffect::Deactivate()
{
	if (!bIsActive) return;
	
	Super::Deactivate();

	check(OwnerCharacter);

	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	MovementComponent->MaxWalkSpeed *= Magnitude;
}
