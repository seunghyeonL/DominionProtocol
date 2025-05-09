// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDeathEffect.h"

UAIDeathEffect::UAIDeathEffect()
{
	StatusEffectTag = EffectTags::Death;
	bIsActive = false;
}

void UAIDeathEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);
}

void UAIDeathEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);
}

void UAIDeathEffect::Deactivate()
{
	Super::Deactivate();
}
