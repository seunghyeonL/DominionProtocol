// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiMonster3DWidget.h"

#include "Components/ProgressBar.h"

void UDomiMonster3DWidget::UpdateMonsterHPBar(float NewHP)
{
	CurrentMonsterHP = NewHP;
	MonsterHPBar->SetPercent(CurrentMonsterHP / MaxMonsterHP);
}

void UDomiMonster3DWidget::UpdateMonsterShield(float NewShield)
{
	CurrentMonsterShield = NewShield;
	MonsterShieldBar->SetPercent(CurrentMonsterShield / MaxMonsterShield);
}

void UDomiMonster3DWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
