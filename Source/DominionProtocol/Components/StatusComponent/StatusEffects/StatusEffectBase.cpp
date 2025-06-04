// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectBase.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "EnumAndStruct/EffectData/EffectInitializeData.h"
#include "Interface/EffectUser.h"

UStatusEffectBase::UStatusEffectBase()
{
	Magnitude = 0.f;
	bIsActive = false;
}

void UStatusEffectBase::Initialize()
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FEffectInitializeData* EffectInitializeData = BaseGameState->GetEffectInitializeData(StatusEffectTag))
			{
				EffectIcon = EffectInitializeData->EffectIcon;
			}
			else
			{
				Debug::PrintError(TEXT("UStatusEffectBase::Initialize : Invalid EffectInitializeData."));
			}
		}
		else
		{
			Debug::PrintError(TEXT("UStatusEffectBase::Initialize : Invalid BaseGameState."));
		}
	}
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
	if (StatusComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
	{
		IEffectUser::Execute_GetEffectUIDatas(StatusComponent->GetOwner());
	}
	
	CachedDuration = -1.f;
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
	if (StatusComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
	{
		IEffectUser::Execute_GetEffectUIDatas(StatusComponent->GetOwner());
	}
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(
		DurationTimer,
		this,
		&UStatusEffectBase::Deactivate,
		Duration,
		false
	);

	CachedDuration = Duration;
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
	if (StatusComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
	{
		IEffectUser::Execute_GetEffectUIDatas(StatusComponent->GetOwner());
	}
	bIsActive = false;
}

void UStatusEffectBase::Tick(float DeltaTime)
{
}

