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
