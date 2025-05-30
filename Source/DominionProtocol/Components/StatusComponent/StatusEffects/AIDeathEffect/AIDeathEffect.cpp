// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDeathEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/WidgetComponent/DomiWidgetComponent.h"

UAIDeathEffect::UAIDeathEffect()
{
	StatusEffectTag = EffectTags::Death;
}

bool UAIDeathEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	// Ignore Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		if (auto SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
		{
			Debug::Print(TEXT("UAIDeathEffect::Activate : StopSkill."));
			SkillComponent->StopSkill();
		}
		
		// OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Visibility);
		OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
		
		if (auto WidgetComponent = OwnerCharacter->FindComponentByClass<UDomiWidgetComponent>())
		{
			WidgetComponent->DestroyComponent();
		}
	}

	return true;
}

bool UAIDeathEffect::Activate(float Duration)
{
	// Super::Activate(Duration);
	
	Debug::Print(TEXT("UAIDeathEffect::Activate : Use Activate function without Duration."));
	return false;
}

void UAIDeathEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
	
	// Set Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}
