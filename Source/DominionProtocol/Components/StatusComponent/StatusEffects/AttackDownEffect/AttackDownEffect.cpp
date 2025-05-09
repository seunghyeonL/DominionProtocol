// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDownEffect.h"

UAttackDownEffect::UAttackDownEffect()
{
	StatusEffectTag = EffectTags::AttackDown;
	bIsActive = false;
}

void UAttackDownEffect::Activate()
{
	Super::Activate();
}

void UAttackDownEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UAttackDownEffect::Deactivate()
{
	Super::Deactivate();
}
