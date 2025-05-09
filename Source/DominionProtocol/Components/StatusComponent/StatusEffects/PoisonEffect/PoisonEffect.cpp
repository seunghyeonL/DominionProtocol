// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonEffect.h"

UPoisonEffect::UPoisonEffect()
{
	StatusEffectTag = EffectTags::Poison;
	bIsActive = false;
}

void UPoisonEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);
}

void UPoisonEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);
}

void UPoisonEffect::Deactivate()
{
	Super::Deactivate();
}
