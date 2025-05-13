// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRunningEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerRunningEffect::UPlayerRunningEffect()
{
	StatusEffectTag = EffectTags::Running;
	SpeedCoefficient = 1.5f;
}

void UPlayerRunningEffect::Activate()
{
	Super::Activate();

	if (IsValid(OwnerCharacter))
	{
		auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
		MovementComponent->MaxWalkSpeed *= SpeedCoefficient;
	}
}

void UPlayerRunningEffect::Activate(float Duration)
{
	Super::Activate(Duration);

	if (IsValid(OwnerCharacter))
	{
		auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
		MovementComponent->MaxWalkSpeed *= SpeedCoefficient;
	}
}

void UPlayerRunningEffect::Deactivate()
{
	Super::Deactivate();

	if (IsValid(OwnerCharacter))
	{
		auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
		MovementComponent->MaxWalkSpeed /= SpeedCoefficient;
	}
}
