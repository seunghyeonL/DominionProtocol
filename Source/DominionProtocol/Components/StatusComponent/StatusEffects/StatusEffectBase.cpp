// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectBase.h"

#include "Components/StatusComponent/StatusComponent.h"

UStatusEffectBase::UStatusEffectBase()
{
	Magnitude = 0.f;
	bIsActive = false;
}

bool UStatusEffectBase::Activate()
{
	if (bIsActive)
	{
		return false;
	}

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return false;
	}
	
	StatusComponent->GetActiveStatusEffectTags().AddTag(StatusEffectTag);
	return bIsActive = true;
}

bool UStatusEffectBase::Activate(float Duration)
{
	if (bIsActive)
	{
		GetOuter()->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(
			DurationTimer,
			this,
			&UStatusEffectBase::Deactivate,
			Duration,
			false
		);
		return false;
	}

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return false;
	}
	
	StatusComponent->GetActiveStatusEffectTags().AddTag(StatusEffectTag);
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(
		DurationTimer,
		this,
		&UStatusEffectBase::Deactivate,
		Duration,
		false
	);
	
	return bIsActive = true;
}

void UStatusEffectBase::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}

	GetOuter()->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return;
	}
	
	StatusComponent->GetActiveStatusEffectTags().RemoveTag(StatusEffectTag);
	bIsActive = false;
}

void UStatusEffectBase::Tick(float DeltaTime)
{
}

