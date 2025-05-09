// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectBase.h"

#include "Components/StatusComponent/StatusComponent.h"

void UStatusEffectBase::Activate(float Magnitude)
{
	if (bIsActive)
	{
		return;
	}

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return;
	}

	bIsActive = true;
	StatusComponent->GetActiveStatusEffectTags().AddTag(StatusEffectTag);
}

void UStatusEffectBase::Activate(float Magnitude, float Duration)
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
		return;
	}

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return;
	}

	bIsActive = true;
	StatusComponent->GetActiveStatusEffectTags().AddTag(StatusEffectTag);
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(
		DurationTimer,
		this,
		&UStatusEffectBase::Deactivate,
		Duration,
		false
	);
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
	
	bIsActive = false;
	StatusComponent->GetActiveStatusEffectTags().RemoveTag(StatusEffectTag);
}

void UStatusEffectBase::Tick(float DeltaTime)
{
}
