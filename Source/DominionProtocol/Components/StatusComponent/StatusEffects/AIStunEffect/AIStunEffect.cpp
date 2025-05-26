// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStunEffect.h"

UAIStunEffect::UAIStunEffect()
{
	StatusEffectTag = EffectTags::Stun;
}

void UAIStunEffect::Activate()
{
	Super::Activate();
	bIsActive = true;
}

void UAIStunEffect::Activate(float Duration)
{
	Super::Activate(Duration);
	bIsActive = true;
}

void UAIStunEffect::Deactivate()
{
	Super::Deactivate();
	bIsActive = false;
}
