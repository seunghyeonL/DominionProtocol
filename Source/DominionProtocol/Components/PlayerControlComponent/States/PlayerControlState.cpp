// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlState.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/InteractableInterface.h"
#include "Player/Characters/DomiCharacter.h"


void UPlayerControlState::Tick(float DeltaTime)
{
	if (!OwnerCharacter) return;

	if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OwnerCharacter))
	{
		// 상태 이펙트가 자동으로 막기 때문에 단순 시간 비교로 이동만 처리
		if (DomiChar->DashElapsed >= DomiChar->DashDuration)
		{
			return; // 이동 완료
		}

		DomiChar->DashElapsed += DeltaTime;

		float Alpha = FMath::Clamp(DomiChar->DashElapsed / DomiChar->DashDuration, 0.f, 1.f);
		FVector NewLocation = FMath::Lerp(DomiChar->DashStartLocation, DomiChar->DashTargetLocation, Alpha);
		DomiChar->SetActorLocation(NewLocation, true);
	}
}


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

	if (!OwnerCharacter) return;

	if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (!DomiChar->StartDash())return;

		DomiChar->DashStartLocation = DomiChar->GetActorLocation();
		DomiChar->DashTargetLocation = DomiChar->DashStartLocation + DomiChar->GetDashDirection() * DomiChar->DashDistance;
		DomiChar->DashElapsed = 0.f;

		Debug::Print(TEXT(">> Dash Started"));
	}
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
		FText UIText = IInteractableInterface::Execute_GetInteractMessage(InteractableObject);
		
	}
	
	Debug::Print(TEXT("UPlayerControlState::Interact : Call."));
}

void UPlayerControlState::RockOn()
{
	Super::RockOn();

	Debug::Print(TEXT("UPlayerControlState::RockOn : Call."));
}
