// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStiffnessEffect.h"

UAIStiffnessEffect::UAIStiffnessEffect()
{
	StatusEffectTag = EffectTags::Stiffness;
	bIsActive = false;
}

void UAIStiffnessEffect::Activate(float Magnitude)
{
	Super::Activate(Magnitude);
}

void UAIStiffnessEffect::Activate(float Magnitude, float Duration)
{
	Super::Activate(Magnitude, Duration);
}

void UAIStiffnessEffect::Deactivate()
{
	Super::Deactivate();
}
