// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnEffect.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/Character.h"

UBurnEffect::UBurnEffect()
{
	StatusEffectTag = EffectTags::Burn;
}

bool UBurnEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	OwnerCharacter->GetWorldTimerManager().SetTimer(
		DoTTimer,
		this,
		&UBurnEffect::ApplyDoTDamage,
		0.25f,
		true
		);
	
	return true;
}

bool UBurnEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	OwnerCharacter->GetWorldTimerManager().SetTimer(
		DoTTimer,
		this,
		&UBurnEffect::ApplyDoTDamage,
		0.25f,
		true
		);
	
	return true;
}

void UBurnEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}

	OwnerCharacter->GetWorldTimerManager().ClearTimer(DoTTimer);
	
	Super::Deactivate();
}

void UBurnEffect::ApplyDoTDamage()
{
	if (auto StatusComponent = Cast<UStatusComponent>(GetOuter()))
	{
		float CurrentHealth = StatusComponent->GetStat(StatTags::Health);
		StatusComponent->SetHealth(CurrentHealth - Magnitude / 4);
	}
}
