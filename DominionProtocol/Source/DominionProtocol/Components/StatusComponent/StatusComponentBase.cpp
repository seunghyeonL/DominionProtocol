// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusComponentBase.h"
#include "StatusComponentUser.h"

UStatusComponentBase::UStatusComponentBase()
{
}

float UStatusComponentBase::GetStat(FGameplayTag StatTag) const
{
	return -1.f;
	// 하위 클래스에서 구현
}

void UStatusComponentBase::SetHealth(float NewHealth)
{
	check(BattleStats.Contains(StatTags::MaxHealth));

	float MaxHealth = BattleStats[StatTags::MaxHealth];
	float ClampedHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	Health = ClampedHealth;
	if (FMath::IsNearlyZero(Health))
	{
		if (auto OwnerCharacter = Cast<IStatusComponentUser>(GetOuter()))
		{
			OwnerCharacter->OnDeath();
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerStatusComponentBase::SetHealth : OwnerCharacter need to implement IStatusComponentUser."));
		}
	}
}

void UStatusComponentBase::ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude)
{
	if (!StatusEffectClasses.Contains(StatusEffectTag))
	{
		Debug::Print(TEXT("StatusEffectTag is not set."));
		return;
	}

	if (ActiveStatusEffectTags.HasTag(StatusEffectTag))
	{
		return;
	}

	// ActiveStatusEffects.Add(StatusEffectTag, NewObject<UStatusEffectBase>(this));
	ActiveStatusEffectTags.AddTag(StatusEffectTag);

	// ActiveStatusEffects[StatusEffectTag].Activate();
}

void UStatusComponentBase::ActivateStatusEffectWithDuration(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration)
{
	if (!StatusEffectClasses.Contains(StatusEffectTag))
	{
		Debug::Print(TEXT("StatusEffectTag is not set."));
		return;
	}

	if (ActiveStatusEffectTags.HasTag(StatusEffectTag))
	{
		return;
	}

	// ActiveStatusEffects.Add(StatusEffectTag, NewObject<UStatusEffectBase>(this));
	ActiveStatusEffectTags.AddTag(StatusEffectTag);

	// ActiveStatusEffects[StatusEffectTag].Activate(Duration);
}

void UStatusComponentBase::DeactivateStatusEffect(const FGameplayTag& StatusEffectTag)
{
	if (!StatusEffectClasses.Contains(StatusEffectTag))
	{
		Debug::Print(TEXT("StatusEffectTag is not set."));
		return;
	}

	if (!ActiveStatusEffectTags.HasTag(StatusEffectTag))
	{
		return;
	}

	// ActiveStatusEffects[StatusEffectTag].Dectivate();
	// ActiveStatusEffects.Remove(StatusEffectTag);
	ActiveStatusEffectTags.RemoveTag(StatusEffectTag);
}
