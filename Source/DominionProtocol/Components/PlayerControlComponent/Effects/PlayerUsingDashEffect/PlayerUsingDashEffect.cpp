// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUsingDashEffect.h"
#include "Player/Characters/DomiCharacter.h"

UPlayerUsingDashEffect::UPlayerUsingDashEffect()
{
	ControlEffectTag = EffectTags::UsingDash;
	bIsActive = false;
	OuterState = nullptr;
	InnerState = nullptr;
}

void UPlayerUsingDashEffect::Activate()
{
	Super::Activate();
}

void UPlayerUsingDashEffect::Activate(float Duration)
{
	Super::Activate(Duration);
	UE_LOG(LogTemp, Warning, TEXT(">> Dash Effect Activated! Duration: %f"), Duration);

}

void UPlayerUsingDashEffect::Deactivate()
{
	Super::Deactivate();
	if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OwnerCharacter)) 
	{
		DomiChar->EndDash();
		UE_LOG(LogTemp, Warning, TEXT(">> Dash End"));
	}
}

void UPlayerUsingDashEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerUsingDashEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerUsingDashEffect::Dash()
{
	// Super::Dash();
}

void UPlayerUsingDashEffect::Sprint()
{
	// Super::Sprint();
}

void UPlayerUsingDashEffect::Parry()
{
	// Super::Parry();
}

void UPlayerUsingDashEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerUsingDashEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerUsingDashEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerUsingDashEffect::Interact()
{
	// Super::Interact();
}

void UPlayerUsingDashEffect::RockOn()
{
	// Super::RockOn();
}
