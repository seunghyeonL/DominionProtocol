// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDownEffect.h"

UAttackDownEffect::UAttackDownEffect()
{
	StatusEffectTag = EffectTags::AttackDown;
	bIsActive = false;
}

void UAttackDownEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);
}

void UAttackDownEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);
}

void UAttackDownEffect::Deactivate()
{
	Super::Deactivate();
}
