// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlState.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/InteractableInterface.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/InGameController.h"
#include "UI/UIInGame/DomiInGameHUDWidget.h"

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

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

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
		
		ControlComponent->SetCurrentMovementVector(FinalMovementNormalVector);
		
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

void UPlayerControlState::SprintStart()
{
	Super::SprintStart();

	auto StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	check(StatusComponent);

	StatusComponent->ActivateStatusEffect(EffectTags::Running, 0.f);
}

void UPlayerControlState::SprintEnd()
{
	Super::SprintEnd();
	
	auto StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	check(StatusComponent);

	StatusComponent->DeactivateStatusEffect(EffectTags::Running);
}

void UPlayerControlState::Parry()
{
	Super::Parry();

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::Parry);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::BaseAttack : Character doesn't implement ISkillComponentUser."));
	}
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

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::WeaponSkill);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::WeaponSkill : Character doesn't implement ISkillComponentUser."));
	}
}

void UPlayerControlState::MagicSkill()
{
	Super::MagicSkill();

	check(OwnerCharacter);

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::MagicSkill);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::MasicSkill : Character doesn't implement ISkillComponentUser."));
	}
}

void UPlayerControlState::Interact()
{
	Super::Interact();

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(PlayerCharacter);

	AActor* InteractableObject = PlayerCharacter->GetCurrentInteractableActor();
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
	check(PlayerControlComponent);
	
	if (PlayerControlComponent->GetActiveControlEffectTags().HasTag(EffectTags::LockOn))
	{
		PlayerControlComponent->DeactivateControlEffect(EffectTags::LockOn);
		PlayerControlComponent->SetLockOnTargetActor(nullptr);
	}
	else
	{
		if (!PlayerControlComponent->SetLockOnTargetActorInPublicSpace())
		{
			PlayerControlComponent->SetLockOnTargetActorInVisibility();
		}

		// ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter);
		if (IsValid(PlayerControlComponent->GetLockOnTargetActor()))
		{
			PlayerControlComponent->ActivateControlEffect(EffectTags::LockOn);
		}
		else
		{
			Debug::Print(TEXT("UPlayerControlState::LockOn : No LockOnTargetActor."));
		}
	}

	Debug::Print(TEXT("UPlayerControlState::LockOn : Call."));
}



void UPlayerControlState::ConsumeItemAction_1()
{
	Super::ConsumeItemAction_1();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->UseConsumableItem(TEXT("ConsumableSlot_Primary")); // 슬롯 인덱스 0
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_1 : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_1 : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}

	Debug::Print(TEXT("UPlayerControlState::ConsumeItemAction_1 : Call."));
}

void UPlayerControlState::ConsumeItemAction_2()
{
	Super::ConsumeItemAction_2();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->UseConsumableItem(TEXT("ConsumableSlot_Secondary")); // 슬롯 인덱스 1 
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_2 : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_2 : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}

	Debug::Print(TEXT("UPlayerControlState::ConsumeItemAction_2 : Call."));
}

void UPlayerControlState::ConsumeItemAction_3()
{
	Super::ConsumeItemAction_3();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->UseConsumableItem(TEXT("ConsumableSlot_Tertiary")); // 슬롯 인덱스 2 사용
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_3 : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_3 : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}

	Debug::Print(TEXT("UPlayerControlState::ConsumeItemAction_3 : Call."));
}

void UPlayerControlState::SwapWeapon()
{
	Super::SwapWeapon();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->SwapWeapons();
			DomiCharacter->PlayWeaponSwapAnimMontage();
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::SwapWeaponsAction : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::SwapWeaponsAction : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}
	Debug::Print(TEXT("UPlayerControlState::SwapWeapon : Call."));
}

void UPlayerControlState::InteractionScroll(const FInputActionValue& Value)
{
	Super::InteractionScroll(Value);

	auto* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	if (PlayerCharacter)
	{
		const float InputValue = Value.Get<float>();
		PlayerCharacter->EventInteractionWidgetScroll(InputValue);
	}

}

void UPlayerControlState::SwitchShowAndHideInventory()
{
	Super::SwitchShowAndHideInventory();

	auto* InGameController = Cast<AInGameController>(OwnerCharacter->GetController());
	if (InGameController)
	{
		InGameController->GetInGameHUDWidget()->OnSwitchShowAndHideInventoryWidget();
	}
}
