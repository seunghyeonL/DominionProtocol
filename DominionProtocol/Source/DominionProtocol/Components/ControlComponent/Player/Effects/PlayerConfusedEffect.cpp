// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerConfusedEffect.h"
#include "InputActionValue.h"

UPlayerConfusedEffect::UPlayerConfusedEffect()
{
	ControlEffectTag = EffectTags::Confused;
	bIsActive = false;
	OuterState = nullptr;
	InnerState = nullptr;
}

void UPlayerConfusedEffect::Activate()
{
	Super::Activate();
}

void UPlayerConfusedEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerConfusedEffect::Deactivate()
{
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

void UPlayerConfusedEffect::Sprint()
{
	Super::Sprint();
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
