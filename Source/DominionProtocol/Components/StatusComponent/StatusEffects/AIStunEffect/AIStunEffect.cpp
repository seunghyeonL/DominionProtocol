// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStunEffect.h"

UAIStunEffect::UAIStunEffect()
{
	StatusEffectTag = EffectTags::Stun;
	bIsActive = false;
}

void UAIStunEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);
}

void UAIStunEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);
}

void UAIStunEffect::Deactivate()
{
	Super::Deactivate();
}
