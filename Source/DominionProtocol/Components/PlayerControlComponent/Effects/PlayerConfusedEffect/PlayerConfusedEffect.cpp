// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerConfusedEffect.h"
#include "InputActionValue.h"

UPlayerConfusedEffect::UPlayerConfusedEffect()
{
	ControlEffectTag = EffectTags::Confused;
}

bool UPlayerConfusedEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	return true;
}

bool UPlayerConfusedEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	return true;
}

void UPlayerConfusedEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}

void UPlayerConfusedEffect::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector2D InvertedMovementVector = { -MovementVector.X, -MovementVector.Y };
	
	Super::Move(FInputActionValue(InvertedMovementVector));
}

void UPlayerConfusedEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerConfusedEffect::Dash()
{
	Super::Dash();
}

void UPlayerConfusedEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerConfusedEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerConfusedEffect::Parry()
{
	Super::Parry();
}

void UPlayerConfusedEffect::BaseAttack()
{
	Super::BaseAttack();
}

void UPlayerConfusedEffect::WeaponSkill()
{
	Super::WeaponSkill();
}

void UPlayerConfusedEffect::MagicSkill()
{
	Super::MagicSkill();
}

void UPlayerConfusedEffect::Interact()
{
	Super::Interact();
}

void UPlayerConfusedEffect::LockOn()
{
	Super::LockOn();
}
