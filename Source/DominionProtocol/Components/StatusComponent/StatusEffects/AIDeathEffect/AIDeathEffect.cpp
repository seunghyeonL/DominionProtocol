// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDeathEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UAIDeathEffect::UAIDeathEffect()
{
	StatusEffectTag = EffectTags::Death;
}

void UAIDeathEffect::Activate()
{
	Super::Activate();
	// Ignore Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void UAIDeathEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UAIDeathEffect::Deactivate()
{
	Super::Deactivate();
	// Set Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}
