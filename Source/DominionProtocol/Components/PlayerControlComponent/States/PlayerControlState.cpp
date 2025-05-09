// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlState.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/InteractableInterface.h"
#include "Player/Characters/DomiCharacter.h"


void UPlayerControlState::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!OwnerCharacter)
	{
		Debug::PrintError(TEXT("UPlayerControlState::Move : Invalid OwnerCharacter."));
		return;
	}

	if (auto Controller = OwnerCharacter->GetController())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		OwnerCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::Move : Invalid Controller."));
	}
}

void UPlayerControlState::Look(const FInputActionValue& Value)
{
	Super::Look(Value);

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!OwnerCharacter)
	{
		Debug::PrintError(TEXT("UPlayerControlState::Look : Invalid OwnerCharacter."));
		return;
	}
	
	if (auto Controller = OwnerCharacter->GetController())
	{
		// add yaw and pitch input to controller
		OwnerCharacter->AddControllerYawInput(LookAxisVector.X);
		OwnerCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::Look : Invalid Controller."));
	}
}

void UPlayerControlState::Dash()
{
	Super::Dash();

	Debug::Print(TEXT("UPlayerControlState::Dash : Call."));
}

void UPlayerControlState::Sprint()
{
	Super::Sprint();

	Debug::Print(TEXT("UPlayerControlState::Sprint : Call."));
}

void UPlayerControlState::Parry()
{
	Super::Parry();

	Debug::Print(TEXT("UPlayerControlState::Parry : Call."));
}

void UPlayerControlState::BaseAttack()
{
	Super::BaseAttack();

	Debug::Print(TEXT("UPlayerControlState::BaseAttack : Call."));
}

void UPlayerControlState::WeaponSkill()
{
	Super::WeaponSkill();

	Debug::Print(TEXT("UPlayerControlState::WeaponSkill : Call."));
}

void UPlayerControlState::MagicSkill()
{
	Super::MagicSkill();

	Debug::Print(TEXT("UPlayerControlState::MagicSkill : Call."));
}

void UPlayerControlState::Interact()
{
	Super::Interact();

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(PlayerCharacter);

	AActor* InteractableObject = PlayerCharacter->GetCurrentInteractableObject();
	if (IsValid(InteractableObject))
	{
		IInteractableInterface::Execute_Interact(InteractableObject, PlayerCharacter);
	}
	
	Debug::Print(TEXT("UPlayerControlState::Interact : Call."));
}
