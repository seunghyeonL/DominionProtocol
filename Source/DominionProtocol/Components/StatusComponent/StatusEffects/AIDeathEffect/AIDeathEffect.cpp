// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDeathEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent/DomiWidgetComponent.h"

UAIDeathEffect::UAIDeathEffect()
{
	StatusEffectTag = EffectTags::Death;
}

void UAIDeathEffect::Activate()
{
	Super::Activate();
	bIsActive = true;
	// Ignore Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Visibility);
		// OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (auto WidgetComponent = OwnerCharacter->FindComponentByClass<UDomiWidgetComponent>())
		{
			WidgetComponent->DestroyComponent();
		}
	}
}

void UAIDeathEffect::Activate(float Duration)
{
	Super::Activate(Duration);
	bIsActive = true;
}

void UAIDeathEffect::Deactivate()
{
	Super::Deactivate();
	bIsActive = false;
	// Set Collision with Pawn
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}
