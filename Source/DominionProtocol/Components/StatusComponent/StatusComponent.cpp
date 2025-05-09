// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusComponent.h"
#include "StatusComponentUser.h"
#include "StatusComponentInitializeData.h"
#include "StatusEffects/StatusEffectBase.h"

UStatusComponent::UStatusComponent()
{
	bWantsInitializeComponent = true;
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// // Initialize BaseStats
	// StatMap.Add(StatTags::LIFE, 0.f);
	// StatMap.Add(StatTags::STR, 0.f);
	// StatMap.Add(StatTags::DEX, 0.f);
	//
	// // Initalize BattleStats
	// StatMap.Add(StatTags::MaxHealth, 100.f);
	// StatMap.Add(StatTags::MaxStamina, 100.f);
	// StatMap.Add(StatTags::AttackPower, 100.f);
	// StatMap.Add(StatTags::Defense, 100.f);
	// StatMap.Add(StatTags::MoveSpeed, 1.f);
	// StatMap.Add(StatTags::MaxGroggyGauge, 100.f);
	//
	// // Initialize VariableStats
	// StatMap.Add(StatTags::Health, GetStat(StatTags::MaxHealth]);
	// StatMap.Add(StatTags::Stamina, GetStat(StatTags::MaxStamina]);
	// StatMap.Add(StatTags::GroggyGauge, GetStat(StatTags::MaxGroggyGauge]);
	//
	// // Initialize StatMultiplierMap
	// StatMultiplierMap.Add(StatTags::MaxHealth, 1.f);
	// StatMultiplierMap.Add(StatTags::MaxStamina, 1.f);
	// StatMultiplierMap.Add(StatTags::AttackPower, 1.f);
	// StatMultiplierMap.Add(StatTags::Defense, 1.f);
	// StatMultiplierMap.Add(StatTags::MoveSpeed, 1.f);
}

void UStatusComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UStatusComponent::GetStat(const FGameplayTag& StatTag) const
{
	if (StatMap.Contains(StatTag))
	{
		return StatMap[StatTag];
	}

	Debug::PrintError(TEXT("UStatusComponent::GetStat : Finding StatTag is not set."));
	return -1.f;
}

void UStatusComponent::SetStat(const FGameplayTag& StatTag, float Value)
{
	if (StatMap.Contains(StatTag))
	{
		StatMap[StatTag] = Value;
		return;
	}

	Debug::PrintError(TEXT("UStatusComponent::SetStat : Finding StatTag is not set."));
}

void UStatusComponent::SetHealth(float NewHealth)
{
	// check(StatMap.Contains(StatTags::MaxHealth));

	const float MaxHealth = GetStat(StatTags::MaxHealth);
	const float ClampedHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	SetStat(StatTags::Health ,ClampedHealth);
	OnHealthChanged.Broadcast(ClampedHealth);
	
	if (FMath::IsNearlyZero(GetStat(StatTags::Health)))
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
	return GetStat(StatTags::Stamina) >= RequiredAmount;
}

void UStatusComponent::ConsumeStamina(float Amount)
{
	SetStat(StatTags::Stamina, FMath::Max(0.f, GetStat(StatTags::Stamina) - Amount));
}

void UStatusComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (auto OnwerCharacter = Cast<IStatusComponentUser>(GetOuter()))
	{
		OnwerCharacter->InitializeStatusComponent();
	}
	else
	{
		Debug::PrintError(TEXT("UStatusComponent::InitializeComponent : OwnerCharacter Cast Failed."));
	}
}

void UStatusComponent::InitializeStatusComponent(const FStatusComponentInitializeData& InitializeData)
{
	const auto& [StatDatas, StatMultiplierDatas, EffectClassDatas] = InitializeData;

	for (auto [StatTag, StatValue] : StatDatas)
	{
		StatMap.Add(StatTag, StatValue);
	}

	for (auto [StatTag, StatMultiplierValue] : StatMultiplierDatas)
	{
		StatMultiplierMap.Add(StatTag, StatMultiplierValue);
	}

	for (auto [StatTag, EffectClass] : EffectClassDatas)
	{
		StatusEffectClassMap.Add(StatTag, EffectClass);
	}
}

void UStatusComponent::RemoveActiveStatusEffect(FGameplayTag StatusEffectTag)
{
	ActiveStatusEffects.Remove(StatusEffectTag);
}

void UStatusComponent::ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude)
{
	if (!StatusEffectClassMap.Contains(StatusEffectTag))
	{
		Debug::PrintError(TEXT("UStatusComponent::ActivateStatusEffectWithDuration : StatusEffectTag is not set."));
		return;
	}
	
	ActiveStatusEffects.Add(StatusEffectTag, NewObject<UStatusEffectBase>(this, StatusEffectClassMap[StatusEffectTag]));
	ActiveStatusEffects[StatusEffectTag]->Activate();
}

void UStatusComponent::ActivateStatusEffectWithDuration(const FGameplayTag& StatusEffectTag, const float Magnitude,
                                                        float Duration)
{
	if (!StatusEffectClassMap.Contains(StatusEffectTag))
	{
		Debug::PrintError(TEXT("UStatusComponent::ActivateStatusEffectWithDuration : StatusEffectTag is not set."));
		return;
	}
	
	ActiveStatusEffects.Add(StatusEffectTag, NewObject<UStatusEffectBase>(this, StatusEffectClassMap[StatusEffectTag]));
	ActiveStatusEffects[StatusEffectTag]->Activate(Duration);
}

void UStatusComponent::DeactivateStatusEffect(const FGameplayTag& StatusEffectTag)
{
	if (!ActiveStatusEffects.Contains(StatusEffectTag))
	{
		Debug::Print(TEXT("UStatusComponent::DeactivateStatusEffect : StatusEffectTag is not Activated."));
		return;
	}

	ActiveStatusEffects[StatusEffectTag]->Deactivate();
}
