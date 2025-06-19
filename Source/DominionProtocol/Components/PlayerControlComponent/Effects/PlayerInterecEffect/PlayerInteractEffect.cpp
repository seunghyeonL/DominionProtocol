// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractEffect.h"
#include "Player/Characters/DomiCharacter.h"

UPlayerInteractEffect::UPlayerInteractEffect()
{
	ControlEffectTag = EffectTags::Interact;
	InteractMontageDuration = 1.0f;
}

bool UPlayerInteractEffect::Activate()
{
	Activate(InteractMontageDuration);
	return true;
}

bool UPlayerInteractEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	
	if (auto DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		DomiCharacter->OnRemoveInteractableActor.Broadcast({});
	}
	
	return true;
}

void UPlayerInteractEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();

	if (auto DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		DomiCharacter->OnAddInteractableActor.Broadcast(DomiCharacter->GetInteractableActorSet());
	}
}

void UPlayerInteractEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
}

void UPlayerInteractEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerInteractEffect::Dash()
{
	// Super::Dash();
}

void UPlayerInteractEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerInteractEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerInteractEffect::Parry()
{
	// Super::Parry();
}

void UPlayerInteractEffect::BaseAttack()
{
	// Super::BaseAttack();
}

void UPlayerInteractEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerInteractEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerInteractEffect::Interact()
{
	// Super::Interact();
}

void UPlayerInteractEffect::LockOn()
{
	// Super::LockOn();
}

void UPlayerInteractEffect::ConsumeItemAction_1()
{
	// Super::ConsumeItemAction_1();
}

void UPlayerInteractEffect::ConsumeItemAction_2()
{
	// Super::ConsumeItemAction_2();
}

void UPlayerInteractEffect::ConsumeItemAction_3()
{
	// Super::ConsumeItemAction_3();
}

void UPlayerInteractEffect::SwapWeapon()
{
	// Super::SwapWeapon();
}
