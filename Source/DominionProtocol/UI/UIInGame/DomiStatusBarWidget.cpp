// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiStatusBarWidget.h"

#include "Components/StatusComponent/StatusComponent.h"

void UDomiStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Check Widget Binding 
	ensure(PlayerHPBar);
	ensure(PlayerStaminaBar);

	// Initialize & DelegateBinding
	AActor* OwningActor = GetOwningPlayerPawn();
	if (OwningActor)
	{
		SetupStatusBarWidget(OwningActor);
		StatusComponent = OwningActor->GetComponentByClass<UStatusComponent>();
		if (StatusComponent)
		{
			StatusComponent->OnStatusEffectsChanged.AddUObject(this, &UDomiStatusBarWidget::UpdateStatusEffectMap);
		}
	}
}

void UDomiStatusBarWidget::UpdatePlayerHPBar(const float NewHP)
{
	AlphaForHPAnim = 0.0f;
	PreHP = CurrentHP;
	CurrentHP = NewHP;
}

void UDomiStatusBarWidget::UpdatePlayerMaxHPBar(const float NewMaxHP)
{
	AlphaForHPAnim = 0.0f;
	PreMaxHP = MaxHP;
	MaxHP = NewMaxHP;
}

void UDomiStatusBarWidget::UpdatePlayerStaminaBar(const float NewStamina)
{
	AlphaForStaminaAnim = 0.0f;
	PreStamina = CurrentStamina;
	CurrentStamina = NewStamina;
}

void UDomiStatusBarWidget::UpdatePlayerMaxStaminaBar(const float NewMaxStamina)
{
	AlphaForStaminaAnim = 0.0f;
	PreStamina = MaxStamina;
	MaxStamina = NewMaxStamina;
}

void UDomiStatusBarWidget::UpdateStatusEffectMap()
{
	StatusEffectMap = StatusComponent->GetStatusEffectMap();

	OnUpdateStatusEffectMap();
}

void UDomiStatusBarWidget::SetupStatusBarWidget(const AActor* OwningActor)
{
	ensure(IsValid(OwningActor));
	
	auto* StatusComp = OwningActor->GetComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		// Initialize 
		MaxHP = StatusComp->GetStat(StatTags::MaxHealth);
		MaxStamina = StatusComp->GetStat(StatTags::MaxStamina);
		CurrentHP = StatusComp->GetStat(StatTags::Health);
		CurrentStamina = StatusComp->GetStat(StatTags::Stamina);

		// Delegate Binding
		StatusComp->OnHealthChanged.AddDynamic(this, &UDomiStatusBarWidget::UpdatePlayerHPBar);
		StatusComp->OnStaminaChanged.AddDynamic(this, &UDomiStatusBarWidget::UpdatePlayerStaminaBar);
		StatusComp->OnMaxHealthChanged.AddDynamic(this, &UDomiStatusBarWidget::UpdatePlayerMaxHPBar);
		StatusComp->OnMaxStaminaChanged.AddDynamic(this, &UDomiStatusBarWidget::UpdatePlayerMaxStaminaBar);
	}
}
