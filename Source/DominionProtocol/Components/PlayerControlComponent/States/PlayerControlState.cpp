// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlState.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/InteractableInterface.h"
#include "Player/Characters/DomiCharacter.h"

UPlayerControlState::UPlayerControlState()
{
	OwnerCharacter = nullptr;
	OuterState = nullptr;
}

void UPlayerControlState::Tick(float DeltaTime)
{
}

void UPlayerControlState::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
	// input is a Vector2D
	FVector2D InputVector = Value.Get<FVector2D>();

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

		// final movement vector
		const FVector FinalMovementNormalVector = (ForwardDirection * InputVector.X  + RightDirection * InputVector.Y).GetSafeNormal();

		if (auto ControlComponentUser = Cast<IControlComponentUser>(OwnerCharacter))
		{
			ControlComponentUser->SetLastMovementVector(FinalMovementNormalVector);
		}

		// add movement
		OwnerCharacter->AddMovementInput(FinalMovementNormalVector);
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

	if (!OwnerCharacter) return;

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::Dash);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::BaseAttack : Character doesn't implement ISkillComponentUser."));
	}
	// if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OwnerCharacter))
	// {
	// 	if (!DomiChar->StartDash()) return;
	//
	// 	DomiChar->DashElapsed = 0.f;
	// 	bDashTickActive = true;
	//
	// 	DomiChar->DashMoveDirection = DomiChar->GetDashDirection();
	// }
}

void UPlayerControlState::Sprint()
{
	Super::Sprint();

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerCharacter))
	{
		auto StatusComponent = StatusComponentUser->GetStatusComponent();
		auto ActiveStatusEffectTags = StatusComponentUser->GetActiveStatusEffectTags();

		if (!ActiveStatusEffectTags.HasTag(EffectTags::Running))
		{
			StatusComponent->ActivateStatusEffect(EffectTags::Running, 0.f);
		}
		else
		{
			StatusComponent->DeactivateStatusEffect(EffectTags::Running);
		}
	}
}

void UPlayerControlState::Parry()
{
	Super::Parry();

	Debug::Print(TEXT("UPlayerControlState::Parry : Call."));
}

void UPlayerControlState::BaseAttack()
{
	Super::BaseAttack();

	// ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(OwnerCharacter);

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::BaseAttack);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::BaseAttack : Character doesn't implement ISkillComponentUser."));
	}

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
		FText UIText = IInteractableInterface::Execute_GetInteractMessage(InteractableObject);
		
	}
	
	Debug::Print(TEXT("UPlayerControlState::Interact : Call."));
}

void UPlayerControlState::LockOn()
{
	Super::LockOn();
	UPlayerControlComponent* PlayerControlComponent= Cast<UPlayerControlComponent>(GetOuter());
	if (PlayerControlComponent->GetActiveControlEffectTags().HasTag(EffectTags::LockOn))
	{
		PlayerControlComponent->DeactivateControlEffect(EffectTags::LockOn);
	}
	else
	{
		PlayerControlComponent->ActivateControlEffect(EffectTags::LockOn);
	}

	Debug::Print(TEXT("UPlayerControlState::LockOn : Call."));
}
