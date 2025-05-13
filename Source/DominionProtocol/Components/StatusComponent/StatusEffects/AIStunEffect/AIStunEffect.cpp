// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStunEffect.h"

UAIStunEffect::UAIStunEffect()
{
	StatusEffectTag = EffectTags::Stun;
}

void UAIStunEffect::Activate()
{
	Super::Activate();
}

void UAIStunEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UAIStunEffect::Deactivate()
{
	Super::Deactivate();
}
