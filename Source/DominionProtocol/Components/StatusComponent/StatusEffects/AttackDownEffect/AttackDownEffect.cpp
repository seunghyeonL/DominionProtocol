// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDownEffect.h"

UAttackDownEffect::UAttackDownEffect()
{
	StatusEffectTag = EffectTags::AttackDown;
}

void UAttackDownEffect::Activate()
{
	Super::Activate();
	bIsActive = true;
}

void UAttackDownEffect::Activate(float Duration)
{
	Super::Activate(Duration);
	bIsActive = true;
}

void UAttackDownEffect::Deactivate()
{
	Super::Deactivate();
	bIsActive = false;
}
