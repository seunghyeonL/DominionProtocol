// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStiffnessEffect.h"

UAIStiffnessEffect::UAIStiffnessEffect()
{
	StatusEffectTag = EffectTags::Stiffness;
	bIsActive = false;
}

void UAIStiffnessEffect::Activate()
{
	Super::Activate();
	bIsActive = true;
}

void UAIStiffnessEffect::Activate(float Duration)
{
	Super::Activate(Duration);
	bIsActive = true;
}

void UAIStiffnessEffect::Deactivate()
{
	Super::Deactivate();
	bIsActive = false;
}
