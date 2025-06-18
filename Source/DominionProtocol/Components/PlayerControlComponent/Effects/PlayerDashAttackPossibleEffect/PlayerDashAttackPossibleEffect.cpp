// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashAttackPossibleEffect.h"

UPlayerDashAttackPossibleEffect::UPlayerDashAttackPossibleEffect()
{
	ControlEffectTag = EffectTags::DashAttackPossible;
}

bool UPlayerDashAttackPossibleEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	return true;
}

bool UPlayerDashAttackPossibleEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	
	return true;
}

void UPlayerDashAttackPossibleEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}

void UPlayerDashAttackPossibleEffect::BaseAttack()
{
	DashAttack();
}
