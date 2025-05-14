// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/BossMonsterHPBarWidget.h"

void UBossMonsterHPBarWidget::SetActiveBossBattle(const bool NewValue)
{
	bActiveBossBattle = NewValue;
	CreateBossMonsterHPBarWidget();
}

void UBossMonsterHPBarWidget::UpdateBossMonsterHP(const float NewHP)
{
	AlphaForBossMonsterHPAnim = 0.0f;
	PreBossMonsterHP = CurrentBossMonsterHP;
	CurrentBossMonsterHP = NewHP;
}

void UBossMonsterHPBarWidget::UpdateBossMonsterShield(const float NewShield)
{
	AlphaForBossMonsterShieldAnim = 0.0f;
	PreBossMonsterShield = CurrentBossMonsterShield;
	CurrentBossMonsterShield = NewShield;
}

void UBossMonsterHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}
