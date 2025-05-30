// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonEffect.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/Character.h"
#include "Player/Damagable.h"

UPoisonEffect::UPoisonEffect()
{
	StatusEffectTag = EffectTags::Poison;
}

bool UPoisonEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	OwnerCharacter->GetWorldTimerManager().SetTimer(
		DoTTimer,
		this,
		&UPoisonEffect::ApplyDoTDamage,
		0.5f,
		true
		);
	
	return true;
}

bool UPoisonEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	OwnerCharacter->GetWorldTimerManager().SetTimer(
		DoTTimer,
		this,
		&UPoisonEffect::ApplyDoTDamage,
		0.25f,
		true
		);
	
	return true;
}

void UPoisonEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}

	OwnerCharacter->GetWorldTimerManager().ClearTimer(DoTTimer);
	
	Super::Deactivate();
}

void UPoisonEffect::ApplyDoTDamage()
{
	if (auto StatusComponent = Cast<UStatusComponent>(GetOuter()))
	{
		float CurrentHealth = StatusComponent->GetStat(StatTags::Health);
		StatusComponent->SetHealth(CurrentHealth - Magnitude / 4);
	}
}
