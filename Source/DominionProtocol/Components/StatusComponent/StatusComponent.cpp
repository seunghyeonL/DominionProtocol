// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusComponent.h"
#include "StatusComponentUser.h"
#include "StatusComponentInitializeData.h"
#include "StatusEffects/StatusEffectBase.h"
#include "GameFramework/Character.h"

UStatusComponent::UStatusComponent()
{
	bWantsInitializeComponent = true;
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatusComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UStatusComponent::GetStat(const FGameplayTag& StatTag) const
{
	if (const float* FoundStat = StatMap.Find(StatTag))
	{
		return *FoundStat;
	}

	Debug::PrintError(TEXT("UStatusComponent::GetStat : Finding StatTag is not set."));
	return -1.f;
}

void UStatusComponent::SetStat(const FGameplayTag& StatTag, float Value)
{
	//CheatManager
	if (bIsInfiniteStaminaMode && StatTag == StatTags::Stamina)
	{
		return;
	}
	
	if (float* FoundStat = StatMap.Find(StatTag))
	{
		*FoundStat = Value;
		return;
	}

	Debug::PrintError(TEXT("UStatusComponent::SetStat : Finding StatTag is not set."));
}

void UStatusComponent::SetHealth(float NewHealth)
{
	ensure(StatMap.Contains(StatTags::MaxHealth));

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
	auto OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UStatusComponent::InitializeStatusComponent : Invalid OwnerCharacter."));
		return;
	}
	
	const auto& [StatDatas, StatMultiplierDatas, EffectClassDatas] = InitializeData;

	for (auto [StatTag, StatValue] : StatDatas)
	{
		StatMap.Add(StatTag, StatValue);
	}

	for (auto [StatTag, StatMultiplierValue] : StatMultiplierDatas)
	{
		StatMultiplierMap.Add(StatTag, StatMultiplierValue);
	}

	for (auto [EffectTag, EffectClass] : EffectClassDatas)
	{
		if (UStatusEffectBase* StatusEffect = NewObject<UStatusEffectBase>(this, EffectClass))
		{
			StatusEffectMap.Add(EffectTag, StatusEffect);
			StatusEffect->SetOwnerCharacter(OwnerCharacter);
		}
		else
		{
			FString Msg = FString::Printf(TEXT("UStatusComponent::InitializeStatusComponent : Create %s. "), *EffectTag.ToString());
			Debug::PrintError(Msg);
		}
	}
}

void UStatusComponent::RemoveActiveStatusEffect(FGameplayTag StatusEffectTag)
{
	StatusEffectMap.Remove(StatusEffectTag);
}

void UStatusComponent::ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude)
{
	if (auto StatusEffect = StatusEffectMap.Find(StatusEffectTag))
	{
		(*StatusEffect)->Activate(Magnitude);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerStatusComponent::ActivateStatusEffect : Tag Not Initialized in Mapper."));
	}
}

void UStatusComponent::ActivateStatusEffectWithDuration(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration)
{
	if (auto StatusEffect = StatusEffectMap.Find(StatusEffectTag))
	{
		(*StatusEffect)->Activate(Magnitude, Duration);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerStatusComponent::ActivateStatusEffectWithDuration : Tag Not Initialized in Mapper."));
	}
}

void UStatusComponent::DeactivateStatusEffect(const FGameplayTag& StatusEffectTag)
{
	if (auto StatusEffect = StatusEffectMap.Find(StatusEffectTag))
	{
		(*StatusEffect)->Deactivate();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerStatusComponent::DeactivateStatusEffect : Tag Not Initialized in Mapper."));
	}
}
