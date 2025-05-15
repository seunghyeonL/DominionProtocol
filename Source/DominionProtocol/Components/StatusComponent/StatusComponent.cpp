// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusComponent.h"
#include "StatusComponentUser.h"
#include "StatusComponentInitializeData.h"
#include "StatusEffects/StatusEffectBase.h"
#include "GameFramework/Character.h"

UStatusComponent::UStatusComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	AIState = nullptr;
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatusComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRecoveringStamina)
	{
		float Current = GetStat(StatTags::Stamina);
		float Max = GetStat(StatTags::MaxStamina);

		if (Current < Max)
		{
			SetStamina(Current + StaminaRecoveryRate * DeltaTime);
		}
		else
		{
			bIsRecoveringStamina = false;
		}
	}

	auto ActiveTagArray = ActiveStatusEffectTags.GetGameplayTagArray();
	for (int32 i = 0 ; i < ActiveTagArray.Num(); i++)
	{
		if (ActiveTagArray.IsValidIndex(i))
		{
			StatusEffectMap[ActiveTagArray[i]]->Tick(DeltaTime);
		}
	}
}


float UStatusComponent::GetStat(const FGameplayTag& StatTag) const
{
	if (const float* FoundStat = StatMap.Find(StatTag))
	{
		return *FoundStat;
	}
	
	// Debug::PrintError(FString::Printf(TEXT("UStatusComponent::GetStat : StatTag '%s' is not set."), *StatTag.ToString()));

	return -1.f;
}

void UStatusComponent::SetStat(const FGameplayTag& StatTag, float Value)
{
	//CheatManager
	if (StatTag == StatTags::Stamina && bIsInfiniteStaminaMode)
	{
		return;
	}
	
	if (float* FoundStat = StatMap.Find(StatTag))
	{
		*FoundStat = Value;
		return;
	}

	// Debug::PrintError(TEXT("UStatusComponent::SetStat : Finding StatTag is not set."));
}

void UStatusComponent::SetHealth(float NewHealth)
{
	ensure(StatMap.Contains(StatTags::MaxHealth));

	const float MaxHealth = GetStat(StatTags::MaxHealth);
	const float ClampedHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	SetStat(StatTags::Health ,ClampedHealth);
	OnHealthChanged.Broadcast(ClampedHealth);
	
	// OnDeath 
	if (FMath::IsNearlyZero(GetStat(StatTags::Health)))
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
		else
		{
			Debug::PrintError(TEXT("UStatusComponentBase::SetHealth : OwnerCharacter need to implement IStatusComponentUser."));
		}
	}
}

void UStatusComponent::SetShield(const float NewShield)
{
	ensure(StatMap.Contains(StatTags::MaxShield));

	const float MaxShield = GetStat(StatTags::MaxShield);
	const float ClampedShield = FMath::Clamp(NewShield, 0.f, MaxShield);

	SetStat(StatTags::Shield ,ClampedShield);
	OnShieldChanged.Broadcast(ClampedShield);
}

void UStatusComponent::SetStamina(float NewStamina)
{
	if (!StatMap.Contains(StatTags::MaxStamina))
	{
		Debug::PrintError(TEXT("SetStamina: MaxStamina is not set."));
		return;
	}

	float MaxStamina = GetStat(StatTags::MaxStamina);
	float ClampedStamina = FMath::Clamp(NewStamina, 0.f, MaxStamina);
	SetStat(StatTags::Stamina, ClampedStamina);
	OnStaminaChanged.Broadcast(ClampedStamina);
}


bool UStatusComponent::HasEnoughStamina(const float RequiredAmount) const
{
	return GetStat(StatTags::Stamina) >= RequiredAmount;
}

void UStatusComponent::ConsumeStamina(const float Amount)
{
	SetStamina(GetStat(StatTags::Stamina) - Amount);
	BlockStaminaRecovery();
}

void UStatusComponent::StartStaminaRecovery()
{
	bIsRecoveringStamina = true;
}

void UStatusComponent::StopStaminaRecovery()
{
	bIsRecoveringStamina = false;
	GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryDelayTimer);
}

void UStatusComponent::BlockStaminaRecovery()
{
	bIsRecoveringStamina = false;
	GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryDelayTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryDelayTimer, this, &UStatusComponent::StartStaminaRecovery, StaminaRecoveryDelay, false);
	
	UE_LOG(LogTemp, Warning, TEXT("ConsumeStamina called, Timer set"));
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
		(*StatusEffect)->SetMagnitude(Magnitude);
		(*StatusEffect)->Activate();
	}
	else
	{
		Debug::PrintError(TEXT("UStatusComponent::ActivateStatusEffect : Tag Not Initialized in Mapper."));
	}
}

void UStatusComponent::ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration)
{
	if (auto StatusEffect = StatusEffectMap.Find(StatusEffectTag))
	{
		(*StatusEffect)->SetMagnitude(Magnitude);
		(*StatusEffect)->Activate(Duration);
	}
	else
	{
		Debug::PrintError(TEXT("UStatusComponent::ActivateStatusEffectWithDuration : Tag Not Initialized in Mapper."));
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
		Debug::PrintError(TEXT("UStatusComponent::DeactivateStatusEffect : Tag Not Initialized in Mapper."));
	}
}

