// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusComponent.h"
#include "StatusComponentUser.h"

UStatusComponent::UStatusComponent()
{
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize BaseStats
	StatMap.Add(StatTags::LIFE, 0.f);
	StatMap.Add(StatTags::STR, 0.f);
	StatMap.Add(StatTags::DEX, 0.f);

	// Initalize BattleStats
	StatMap.Add(StatTags::MaxHealth, 100.f);
	StatMap.Add(StatTags::MaxStamina, 100.f);
	StatMap.Add(StatTags::AttackPower, 100.f);
	StatMap.Add(StatTags::Defense, 100.f);
	StatMap.Add(StatTags::MoveSpeed, 1.f);
	StatMap.Add(StatTags::MaxGroggyGauge, 100.f);

	// Initialize VariableStats
	StatMap.Add(StatTags::Health, StatMap[StatTags::MaxHealth]);
	StatMap.Add(StatTags::Stamina, StatMap[StatTags::MaxStamina]);
	StatMap.Add(StatTags::GroggyGauge, StatMap[StatTags::MaxGroggyGauge]);

	// Initialize BattleStatMultipliers
	BattleStatMultipliers.Add(StatTags::MaxHealth, 1.f);
	BattleStatMultipliers.Add(StatTags::MaxStamina, 1.f);
	BattleStatMultipliers.Add(StatTags::AttackPower, 1.f);
	BattleStatMultipliers.Add(StatTags::Defense, 1.f);
	BattleStatMultipliers.Add(StatTags::MoveSpeed, 1.f);
}

void UStatusComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UStatusComponent::GetStat(FGameplayTag StatTag) const
{
	if (StatMap.Contains(StatTags::MaxHealth))
	{
		return StatMap[StatTag];
	}

	Debug::PrintError(TEXT("UStatusComponent::GetStat : Finding StatTag is not set."));
	return -1.f;
}

void UStatusComponent::SetHealth(float NewHealth)
{
	check(StatMap.Contains(StatTags::MaxHealth));

	float MaxHealth = StatMap[StatTags::MaxHealth];
	float ClampedHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	float& Health = StatMap[StatTags::Health];

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

bool UStatusComponent::HasEnoughStamina(float RequiredAmount) const
{
	return StatMap[StatTags::Stamina] >= RequiredAmount;
}

void UStatusComponent::ConsumeStamina(float Amount)
{
	StatMap[StatTags::Stamina] = FMath::Max(0.f, StatMap[StatTags::Stamina] - Amount);
}

void UStatusComponent::ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude)
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

void UStatusComponent::ActivateStatusEffectWithDuration(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration)
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

void UStatusComponent::DeactivateStatusEffect(const FGameplayTag& StatusEffectTag)
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
