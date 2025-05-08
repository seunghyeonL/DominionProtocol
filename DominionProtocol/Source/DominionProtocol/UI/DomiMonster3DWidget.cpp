// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiMonster3DWidget.h"
#include "Components/ProgressBar.h"
#include "Components/StatusComponent/StatusComponent.h"

void UDomiMonster3DWidget::UpdateMonsterHPBar(const float NewHP)
{
	CurrentMonsterHP = NewHP;
	MonsterHPBar->SetPercent(CurrentMonsterHP / MaxMonsterHP);
}

void UDomiMonster3DWidget::UpdateMonsterShield(const float NewShield)
{
	CurrentMonsterShield = NewShield;
	MonsterShieldBar->SetPercent(CurrentMonsterShield / MaxMonsterShield);
}

void UDomiMonster3DWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDomiMonster3DWidget::SetOwningActor(AActor* NewOwner)
{
	Super::SetOwningActor(NewOwner);
	
	// Initialize & Binding
	SetupMonster3dWidget();
}

void UDomiMonster3DWidget::SetupMonster3dWidget()
{
	check(IsValid(OwningActor));
	
	auto* StatusComp = OwningActor->FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		// Initialize
		UpdateMonsterHPBar(StatusComp->GetStat(StatTags::Health));
		UpdateMonsterShield(StatusComp->GetStat(StatTags::Shield));
		
		// DelegateBinding
		
	}
}
