// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDeathEffect.h"

UAIDeathEffect::UAIDeathEffect()
{
	StatusEffectTag = EffectTags::Death;
	bIsActive = false;
}

void UAIDeathEffect::Activate()
{
	Super::Activate();
}

void UAIDeathEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UAIDeathEffect::Deactivate()
{
	Super::Deactivate();
}
