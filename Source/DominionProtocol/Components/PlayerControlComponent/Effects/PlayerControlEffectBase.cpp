// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlEffectBase.h"
#include "Util/DebugHelper.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

UPlayerControlEffectBase::UPlayerControlEffectBase()
{
	bIsActive = false;
	OwnerCharacter = nullptr;
	InnerState = nullptr;
	OuterState = nullptr;
}

void UPlayerControlEffectBase::Activate()
{
	if (bIsActive)
	{
		return;
	}

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	if (!IsValid(ControlComponent))
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Activate : Invalid ControlComponent"));
		return;
	}
	
	if (UPlayerControlStateBase* PlayerControlState = ControlComponent->GetPlayerControlState())
	{
		bIsActive = true;

		SetInnerState(PlayerControlState);
		PlayerControlState->SetOuterState(this);
		ControlComponent->SetPlayerControlState(this);
		ControlComponent->GetActiveControlEffectTags().AddTag(ControlEffectTag);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Activate : Invalid ControlState"));
	}
}

void UPlayerControlEffectBase::Activate(float Duration)
{
	if (bIsActive)
	{
		GetOuter()->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(
			DurationTimer,
			this,
			&UPlayerControlEffectBase::Deactivate,
			Duration,
			false
		);
		return;
	}

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	if (!IsValid(ControlComponent))
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Activate : Invalid ControlComponent"));
		return;
	}
	
	if (UPlayerControlStateBase* PlayerControlState = ControlComponent->GetPlayerControlState())
	{
		bIsActive = true;
		
		SetInnerState(PlayerControlState);
		PlayerControlState->SetOuterState(this);
		ControlComponent->SetPlayerControlState(this);
		ControlComponent->GetActiveControlEffectTags().AddTag(ControlEffectTag);
		GetOuter()->GetWorld()->GetTimerManager().SetTimer(
			DurationTimer,
			this,
			&UPlayerControlEffectBase::Deactivate,
			Duration,
			false
		);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Activate : Invalid ControlState"));
	}
}

void UPlayerControlEffectBase::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}

	GetOuter()->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
	
	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	if (!IsValid(ControlComponent))
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Deactivate : Invalid ControlComponent"));
		return;
	}
	
	if (!IsValid(InnerState))
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Deactivate : Invalid InnerState"));
		return;
	}

	// Connect OuterState and InnerState
	InnerState->SetOuterState(OuterState);
	if (auto CastedOuterState = Cast<UPlayerControlEffectBase>(OuterState))
	{
		CastedOuterState->SetInnerState(InnerState);
	}
	else
	{
		ControlComponent->SetPlayerControlState(InnerState);
	}

	// Disconnect this
	SetInnerState(nullptr);
	SetOuterState(nullptr);

	bIsActive = false;
	ControlComponent->GetActiveControlEffectTags().RemoveTag(ControlEffectTag);
}

void UPlayerControlEffectBase::Tick(float DeltaTime)
{
	check(IsValid(InnerState));

	InnerState->Tick(DeltaTime);
}

void UPlayerControlEffectBase::Move(const FInputActionValue& Value)
{
	if (IsValid(InnerState))
	{
		InnerState->Move(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Move : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::Look(const FInputActionValue& Value)
{
	if (IsValid(InnerState))
	{
		InnerState->Look(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Look : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::Dash()
{
	if (IsValid(InnerState))
	{
		InnerState->Dash();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Dash : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::SprintStart()
{
	if (IsValid(InnerState))
	{
		InnerState->SprintStart();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::SprintStart : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::SprintEnd()
{
	if (IsValid(InnerState))
	{
		InnerState->SprintEnd();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::SprintEnd : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::Parry()
{
	if (IsValid(InnerState))
	{
		InnerState->Parry();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Parry : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::BaseAttack()
{
	if (IsValid(InnerState))
	{
		InnerState->BaseAttack();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::BaseAttack : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::WeaponSkill()
{
	if (IsValid(InnerState))
	{
		InnerState->WeaponSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::WeaponSkill : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::MagicSkill()
{
	if (IsValid(InnerState))
	{
		InnerState->MagicSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::MagicSkill : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::Interact()
{
	Super::Interact();

	if (IsValid(InnerState))
	{
		InnerState->Interact();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Interact : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::LockOn()
{
	Super::LockOn();

	if (IsValid(InnerState))
	{
		InnerState->LockOn();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::LockOn : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::ConsumeItemAction_1()
{
	Super::ConsumeItemAction_1();
	if (IsValid(InnerState))
	{
		InnerState->ConsumeItemAction_1();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Consume Slot 1 : Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::ConsumeItemAction_2()
{
	Super::ConsumeItemAction_1();
	if (IsValid(InnerState))
	{
		InnerState->ConsumeItemAction_2();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Consume Slot 2: Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::ConsumeItemAction_3()
{
	Super::ConsumeItemAction_1();
	if (IsValid(InnerState))
	{
		InnerState->ConsumeItemAction_3();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Consume Slot 3: Invalid InnerState"));
	}
}

void UPlayerControlEffectBase::SwapWeapon()
{
	Super::ConsumeItemAction_1();
	if (IsValid(InnerState))
	{
		InnerState->SwapWeapon();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Swap weapon : Invalid InnerState"));
	}
}
