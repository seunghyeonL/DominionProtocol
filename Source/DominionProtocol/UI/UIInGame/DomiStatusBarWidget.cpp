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
		SetupHUDWidget(OwningActor);	
	}
}

void UDomiStatusBarWidget::UpdatePlayerHPBar(const float NewHP)
{
	AlphaForHPAnim = 0.0f;
	PreHP = CurrentHP;
	CurrentHP = NewHP;
}

void UDomiStatusBarWidget::UpdatePlayerStaminaBar(const float NewStamina)
{
	AlphaForStaminaAnim = 0.0f;
	PreStamina = CurrentStamina;
	CurrentStamina = NewStamina;
}


void UDomiStatusBarWidget::SetupHUDWidget(const AActor* OwningActor)
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
	}
}
