// Fill out your copyright notice in the Description page of Project Settings.


#include "AIConfusedEffect.h"

UAIConfusedEffect::UAIConfusedEffect()
{
	StatusEffectTag = EffectTags::Confused;
	bIsActive = false;
}

void UAIConfusedEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);
}

void UAIConfusedEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);
}

void UAIConfusedEffect::Deactivate()
{
	Super::Deactivate();
}
