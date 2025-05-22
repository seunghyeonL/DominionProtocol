// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiAbilitySlotsWidget.h"
#include "Components/SkillComponent/SkillComponent.h"

void UDomiAbilitySlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindUseWeaponSkillEvent();
	BindUseMagicSkillEvent();
}

void UDomiAbilitySlotsWidget::BindUseWeaponSkillEvent()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		auto* SkillComp = PlayerCharacter->FindComponentByClass<USkillComponent>();
		if (SkillComp)
		{
			//SkillComp->OnUseWeaponSkill.AddUObject(this, &UDomiAbilitySlotsWidget::
		}
	}
}

void UDomiAbilitySlotsWidget::BindUseMagicSkillEvent()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		auto* SkillComp = PlayerCharacter->FindComponentByClass<USkillComponent>();
		if (SkillComp)
		{
			//SkillComp->OnUseWeaponSkill.AddUObject(this, &UDomiAbilitySlotsWidget::
		}
	}
}

void UDomiAbilitySlotsWidget::OnUseWeaponSkill()
{
}

void UDomiAbilitySlotsWidget::OnUseMagicSkill()
{
}