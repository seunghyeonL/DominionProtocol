// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectBase.h"

#include "Components/StatusComponent/StatusComponent.h"

UStatusEffectBase::UStatusEffectBase()
{
	Magnitude = 0.f;
	bIsActive = false;
}

void UStatusEffectBase::Activate()
{
	if (bIsActive)
	{
		return;
	}
	bIsActive = true;

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return;
	}
	
	StatusComponent->GetActiveStatusEffectTags().AddTag(StatusEffectTag);
}

void UStatusEffectBase::Activate(float Duration)
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
	bIsActive = true;

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return;
	}
	
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
	bIsActive = false;
	// Debug::Print(FString::Printf(TEXT("UStatusEffectBase::Deactivate : bIsActive : %d"), bIsActive ? 1 : 0));
	GetOuter()->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("UStatusEffectBase::Activate : Invalid StatusComponent"));
		return;
	}
	
	StatusComponent->GetActiveStatusEffectTags().RemoveTag(StatusEffectTag);
}

void UStatusEffectBase::Tick(float DeltaTime)
{
}
