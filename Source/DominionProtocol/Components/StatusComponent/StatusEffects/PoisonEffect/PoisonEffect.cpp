// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonEffect.h"

UPoisonEffect::UPoisonEffect()
{
	StatusEffectTag = EffectTags::Poison;
}

bool UPoisonEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	return true;
}

bool UPoisonEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	return true;
}

void UPoisonEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}
