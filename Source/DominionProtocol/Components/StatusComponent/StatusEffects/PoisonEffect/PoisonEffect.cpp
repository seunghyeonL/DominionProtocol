// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonEffect.h"

UPoisonEffect::UPoisonEffect()
{
	StatusEffectTag = EffectTags::Poison;
	bIsActive = false;
}

void UPoisonEffect::Activate()
{
	Super::Activate();
}

void UPoisonEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPoisonEffect::Deactivate()
{
	Super::Deactivate();
}
