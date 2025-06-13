// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPreStunEffect.h"

#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Interface/EffectUser.h"
#include "Util/DebugHelper.h"

UPlayerPreStunEffect::UPlayerPreStunEffect()
{
	ControlEffectTag = EffectTags::PreStun;
	CachedDuration = 100.f;

	DurationPerAttack = 20.f;
}

bool UPlayerPreStunEffect::Activate()
{
	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

	if (ControlComponent->GetActiveControlEffectTags().HasTag(EffectTags::Stun))
	{
		return false;
	}
	
	if (bIsActive)
	{
		GetOuter()->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
		DurationRemained += DurationPerAttack;

		// 게이지 100 넘으면 해제하고 스턴으로
		if (DurationRemained > CachedDuration)
		{
			Deactivate();
			return false;
		}
		
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(
			DurationTimer,
			this,
			&UPlayerControlEffectBase::Deactivate,
			CachedDuration,
			false
		);
		
		
		if (ControlComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
		{
			IEffectUser::Execute_SendEffectUIDatas(ControlComponent->GetOwner());
		}
		
		return false;
	}
	
	if (UPlayerControlStateBase* PlayerControlState = ControlComponent->GetPlayerControlState())
	{
		SetInnerState(PlayerControlState);
		PlayerControlState->SetOuterState(this);
		ControlComponent->SetPlayerControlState(this);
		ControlComponent->GetActiveControlEffectTags().AddTag(ControlEffectTag);

		bIsActive = true;
		DurationRemained = DurationPerAttack;
		
		if (ControlComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
		{
			IEffectUser::Execute_SendEffectUIDatas(ControlComponent->GetOwner());
		}
		
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(
			DurationTimer,
			this,
			&UPlayerControlEffectBase::Deactivate,
			DurationPerAttack,
			false
		);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Activate : Invalid ControlState"));
	}

	return true;
}

bool UPlayerPreStunEffect::Activate(float Duration)
{
	// Not Used;
	Activate();
	
	return false;
}

void UPlayerPreStunEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
	
	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

	CachedDuration = 100.f;
	DurationRemained = 0.f;
	ControlComponent->ActivateControlEffect(EffectTags::Stun, 3.0f);
}
