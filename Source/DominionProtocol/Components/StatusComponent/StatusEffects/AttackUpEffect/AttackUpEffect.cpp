// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackUpEffect.h"

UAttackUpEffect::UAttackUpEffect()
{
	StatusEffectTag = EffectTags::AttackDown;
}

bool UAttackUpEffect::Activate()
{
	return Super::Activate();
}

bool UAttackUpEffect::Activate(float Duration)
{
	return Super::Activate(Duration);
}

void UAttackUpEffect::Deactivate()
{
	Super::Deactivate();
}
