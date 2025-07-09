// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDownEffect.h"

UAttackDownEffect::UAttackDownEffect()
{
	StatusEffectTag = EffectTags::AttackDown;
}

bool UAttackDownEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	return true;
}

bool UAttackDownEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	return true;
}

void UAttackDownEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}
