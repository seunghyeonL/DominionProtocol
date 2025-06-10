// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusComponent.h"
#include "StatusComponentUser.h"
#include "StatusComponentInitializeData.h"
#include "StatusEffects/StatusEffectBase.h"
#include "GameFramework/Character.h"
#include "Interface/PawnTagInterface.h"

UStatusComponent::UStatusComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	AIState = nullptr;
}

void UStatusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<FGameplayTag> StatusEffectTags;
	StatusEffectMap.GetKeys(StatusEffectTags);
	for (auto EffectTag : StatusEffectTags)
	{
		StatusEffectMap[EffectTag]->Deactivate();
	}
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatusComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRecoveringStamina && !ActiveStatusEffectTags.HasTag(EffectTags::Running))
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
	for (int32 i = 0; i < ActiveTagArray.Num(); ++i)
	{
		const FGameplayTag& Tag = ActiveTagArray[i];
		if (StatusEffectMap.Contains(Tag))
		{
			StatusEffectMap[Tag]->Tick(DeltaTime);
		}
	}
}

TArray<FEffectUIData> UStatusComponent::GetEffectUIDatas()
{
	TArray<FEffectUIData> EffectUIDatas;
	FGameplayTagContainer BuffDebuffContainer;
	
	BuffDebuffContainer.AddTag(EffectTags::StatusBuff);
	BuffDebuffContainer.AddTag(EffectTags::StatusDebuff);
	
	for (const auto& [EffectTag, StatusEffect] : StatusEffectMap)
	{
		if (EffectTag.MatchesAny(BuffDebuffContainer))
		{
			if(StatusEffect->IsActive())
			{
				EffectUIDatas.Add(StatusEffect->GetEffectUIData());
			}
		}
	}

	return EffectUIDatas;
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

void UStatusComponent::GetPlayerStatData(TMap<FGameplayTag, float>& UIPlayerStatData)
{
	for (auto& [UIStatTag, UIStatValue ] : UIPlayerStatData)
	{
		UIStatValue = GetStat(UIStatTag);
	}
}

void UStatusComponent::UpdateStatPreviewData(TMap<FGameplayTag, float>& UIPlayerStatData)
{
	for (auto& [UIStatTag, UIStatValue ] : UIPlayerStatData)
	{
		if (UIStatTag.MatchesTag(StatTags::BaseStat) && UIStatValue < GetStat(UIStatTag))
		{
			UIStatValue = GetStat(UIStatTag);
		}
	}

	for (auto& [UIStatTag, UIStatValue ] : UIPlayerStatData)
	{
		if (UIStatTag.MatchesTag(StatTags::BattleStat))
		{
			UIStatValue = GetCalculatedBattleStat(UIStatTag, UIPlayerStatData);	
		}
	}
}

void UStatusComponent::DecideStatChangeFromUI(const TMap<FGameplayTag, float>& UIPlayerStatData)
{
	for (const auto& [UIStatTag, UIStatValue ] : UIPlayerStatData)
	{
		SetStat(UIStatTag, UIPlayerStatData[UIStatTag]);
	}
}

float UStatusComponent::GetCalculatedBattleStat(const FGameplayTag& StatTag, const TMap<FGameplayTag, float>& InStatMap) const
{
	float result = 0.f;
	if (StatTag.MatchesTag(StatTags::AttackPower))
	{
		result = GetStat(StatTags::BaseAttackPower) + 0.9f * FMath::Sqrt(InStatMap[StatTags::STR]);
	}
	else if (StatTag.MatchesTag(StatTags::SubAttackPower))
	{
		result = GetStat(StatTags::BaseAttackPower) + 0.8f * FMath::Sqrt(InStatMap[StatTags::STR]);
	}
	else if (StatTag.MatchesTag(StatTags::MagicPower))
	{
		result = 100.f + 0.9f * FMath::Sqrt(InStatMap[StatTags::SPL]);
	}
	else if (StatTag.MatchesTag(StatTags::MaxHealth))
	{
		result = 500.f + 100.f + FMath::Sqrt(InStatMap[StatTags::LIFE]);
	}
	else if (StatTag.MatchesTag(StatTags::MaxStamina))
	{
		result = 80.f + 25.f * FMath::Sqrt(InStatMap[StatTags::END]);
	}
	return result;
}

void UStatusComponent::SetHealth(float NewHealth)
{
	ensure(StatMap.Contains(StatTags::MaxHealth));

	const float MaxHealth = GetStat(StatTags::MaxHealth);
	const float ClampedHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	SetStat(StatTags::Health ,ClampedHealth);
	OnHealthChanged.Broadcast(ClampedHealth);

	if (GetOwner()->GetClass()->ImplementsInterface(UPawnTagInterface::StaticClass()))
	{
		const FString CharacterName = IPawnTagInterface::Execute_GetPawnName(GetOwner());
		OnBattleMonster.Broadcast(CharacterName);
	}
	
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

void UStatusComponent::SetMaxHealth(float NewMaxHealth)
{
	ensure(StatMap.Contains(StatTags::MaxHealth));

	const float MaxHealth = NewMaxHealth;

	SetStat(StatTags::MaxHealth, NewMaxHealth);
	OnMaxHealthChanged.Broadcast(NewMaxHealth);
	if (GetOwner()->GetClass()->ImplementsInterface(UPawnTagInterface::StaticClass()))
	{
		const FString CharacterName = IPawnTagInterface::Execute_GetPawnName(GetOwner());
		OnBattleMonster.Broadcast(CharacterName);
	}
}

void UStatusComponent::SetShield(const float NewShield)
{
	ensure(StatMap.Contains(StatTags::MaxShield));

	const float MaxShield = GetStat(StatTags::MaxShield);
	const float ClampedShield = FMath::Clamp(NewShield, 0.f, MaxShield);

	SetStat(StatTags::Shield ,ClampedShield);
	OnShieldChanged.Broadcast(ClampedShield);
	if (GetOwner()->GetClass()->ImplementsInterface(UPawnTagInterface::StaticClass()))
	{
		const FString CharacterName = IPawnTagInterface::Execute_GetPawnName(GetOwner());
		OnBattleMonster.Broadcast(CharacterName);
	}
}

void UStatusComponent::SetStamina(float NewStamina)
{
	if (!StatMap.Contains(StatTags::MaxStamina))
	{
		Debug::PrintError(TEXT("SetStamina: MaxStamina is not set."));
		return;
	}

	if (bIsInfiniteStaminaMode)
	{
		Debug::PrintError(TEXT("SetStamina: Infinite stamina on"));
		return;
	}

	float MaxStamina = GetStat(StatTags::MaxStamina);
	float ClampedStamina = FMath::Clamp(NewStamina, 0.f, MaxStamina);
	SetStat(StatTags::Stamina, ClampedStamina);
	OnStaminaChanged.Broadcast(ClampedStamina);
}

void UStatusComponent::SetMaxStamina(float NewMaxStamina)
{
	ensure(StatMap.Contains(StatTags::MaxStamina));

	const float MaxStamina = NewMaxStamina;

	SetStat(StatTags::MaxStamina, NewMaxStamina);
	OnMaxStaminaChanged.Broadcast(NewMaxStamina);
}

void UStatusComponent::SetAttackPower(float NewAttackPower)
{
	ensure(StatMap.Contains(StatTags::AttackPower));

	const float AttackPower = NewAttackPower;

	SetStat(StatTags::AttackPower, NewAttackPower);
	OnAttackPowerChanged.Broadcast(NewAttackPower);
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
			StatusEffect->Initialize();
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
	if (ImmuneStatusEffectTags.HasTag(StatusEffectTag))
	{
		Debug::Print(FString::Printf(TEXT("UStatusComponent::ActivateStatusEffect: Character is immune to '%s'."), *StatusEffectTag.ToString()));
		return;
	}
	if (auto StatusEffect = StatusEffectMap.Find(StatusEffectTag))
	{
		(*StatusEffect)->SetMagnitude(Magnitude);
		(*StatusEffect)->Activate();
		OnStatusEffectsChanged.Broadcast();
	}
	else
	{
		Debug::PrintError(TEXT("UStatusComponent::ActivateStatusEffect : Tag Not Initialized in Mapper."));
	}
}

void UStatusComponent::ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration)
{
	if (ImmuneStatusEffectTags.HasTag(StatusEffectTag))
	{
		Debug::Print(FString::Printf(TEXT("UStatusComponent::ActivateStatusEffectWithDuration: Character is immune to '%s'."), *StatusEffectTag.ToString()));
		return;
	}
	if (auto StatusEffect = StatusEffectMap.Find(StatusEffectTag))
	{
		(*StatusEffect)->SetMagnitude(Magnitude);
		(*StatusEffect)->Activate(Duration);
		OnStatusEffectsChanged.Broadcast();
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

void UStatusComponent::AddImmuneStatusEffect(const FGameplayTag& StatusEffectTag)
{
	ImmuneStatusEffectTags.AddTag(StatusEffectTag);
	if (ActiveStatusEffectTags.HasTag(StatusEffectTag))
	{
		DeactivateStatusEffect(StatusEffectTag);
		Debug::Print(FString::Printf(TEXT("UStatusComponent: Character gained immunity to '%s'. Deactivated active effect."), *StatusEffectTag.ToString()));
	}
	else
	{
		Debug::Print(FString::Printf(TEXT("UStatusComponent: Character gained immunity to '%s'."), *StatusEffectTag.ToString()));
	}
}

void UStatusComponent::RemoveImmuneStatusEffect(const FGameplayTag& StatusEffectTag)
{
	ImmuneStatusEffectTags.RemoveTag(StatusEffectTag);
	Debug::Print(FString::Printf(TEXT("UStatusComponent: Character lost immunity to '%s'."), *StatusEffectTag.ToString()));
}