// Fill out your copyright notice in the Description page of Project Settings.


#include "AIConfusedEffect.h"

UAIConfusedEffect::UAIConfusedEffect()
{
	StatusEffectTag = EffectTags::Confused;
	bIsActive = false;
}

void UAIConfusedEffect::Activate()
{
	Super::Activate();
}

void UAIConfusedEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UAIConfusedEffect::Deactivate()
{
	Super::Deactivate();
}
