// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRunningEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerRunningEffect::UPlayerRunningEffect()
{
	StatusEffectTag = EffectTags::Running;
	bIsActive = false;
	SpeedCoefficient = 1.5f;
}

void UPlayerRunningEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);

	if (IsValid(OwnerCharacter))
	{
		auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
		MovementComponent->MaxWalkSpeed *= SpeedCoefficient;
	}
}

void UPlayerRunningEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);

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
