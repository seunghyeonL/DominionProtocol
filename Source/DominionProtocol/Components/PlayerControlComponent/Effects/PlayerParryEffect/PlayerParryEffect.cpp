// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerParryEffect.h"
#include "Player/Characters/DomiCharacter.h"

UPlayerParryEffect::UPlayerParryEffect()
{
	ControlEffectTag = EffectTags::Parry;
}

bool UPlayerParryEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

	// 노티파이에서 ParryEffect를 Activate시키는 순간 StopSkill 될 경우에 Deactivate 되지 않는 현상 때문에 넣은 안전코드
	if (!ControlComponent->GetActiveControlEffectTags().HasTag(EffectTags::UsingParry))
	{
		return false;
	}
	
	auto DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(DomiCharacter);
	
	DomiCharacter->ShowParryWall();

	return true;
}

bool UPlayerParryEffect::Activate(float Duration)
{
	Activate();
	return true;
}

void UPlayerParryEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();

	auto DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(DomiCharacter);

	DomiCharacter->HideParryWall();
}
