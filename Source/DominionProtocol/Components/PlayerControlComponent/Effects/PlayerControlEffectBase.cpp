// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlEffectBase.h"
#include "Util/DebugHelper.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "EnumAndStruct/EffectData/EffectInitializeData.h"
#include "Interface/EffectUser.h"

UPlayerControlEffectBase::UPlayerControlEffectBase()
{
	bIsActive = false;
	OwnerCharacter = nullptr;
	InnerState = nullptr;
	OuterState = nullptr;
	CachedDuration = 0.f;
	DurationRemained = 0.f;
	EffectIcon = nullptr;
}

void UPlayerControlEffectBase::Initialize()
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FEffectInitializeData* EffectInitializeData = BaseGameState->GetEffectInitializeData(ControlEffectTag))
			{
				EffectIcon = EffectInitializeData->EffectIcon;
			}
			else
			{
				Debug::PrintError(TEXT("UPlayerControlEffectBase::Initialize : Invalid EffectInitializeData."));
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlEffectBase::Initialize : Invalid BaseGameState."));
		}
	}
}

bool UPlayerControlEffectBase::Activate()
{
	if (bIsActive)
	{
		return false;
	}

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);
	
	if (UPlayerControlStateBase* PlayerControlState = ControlComponent->GetPlayerControlState())
	{
		SetInnerState(PlayerControlState);
		PlayerControlState->SetOuterState(this);
		ControlComponent->SetPlayerControlState(this);
		ControlComponent->GetActiveControlEffectTags().AddTag(ControlEffectTag);
		
		bIsActive = true;
		CachedDuration = 0.f;
		DurationRemained = 0.f;
		if (ControlComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
		{
			IEffectUser::Execute_SendEffectUIDatas(ControlComponent->GetOwner());
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::Activate : Invalid ControlState"));
	}

	return true;
}

bool UPlayerControlEffectBase::Activate(float Duration)
{
	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);
	
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

		CachedDuration = Duration;
		DurationRemained = Duration;
		if (ControlComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
		{
			IEffectUser::Execute_SendEffectUIDatas(ControlComponent->GetOwner());
		}
		
		return false;
	}
	
	if (UPlayerControlStateBase* PlayerControlState = ControlComponent->GetPlayerControlState())
	{
		SetInnerState(PlayerControlState);
		PlayerControlState->SetOuterState(this);
		ControlComponent->SetPlayerControlState(this);
		ControlComponent->GetActiveControlEffectTags().AddTag(ControlEffectTag);

		bIsActive = true;
		CachedDuration = Duration;
		DurationRemained = Duration;
		if (ControlComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
		{
			IEffectUser::Execute_SendEffectUIDatas(ControlComponent->GetOwner());
		}
		
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

	return true;
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
	CachedDuration = 0.f;
	DurationRemained = 0.f;
	ControlComponent->GetActiveControlEffectTags().RemoveTag(ControlEffectTag);
	
	if (ControlComponent->GetOwner()->GetClass()->ImplementsInterface(UEffectUser::StaticClass()))
	{
		IEffectUser::Execute_SendEffectUIDatas(ControlComponent->GetOwner());
	}
}

void UPlayerControlEffectBase::Tick(float DeltaTime)
{
	check(IsValid(InnerState));
	
	InnerState->Tick(DeltaTime);
	if (DurationRemained > 0.f)
	{
		DurationRemained -= DeltaTime;
	}
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

void UPlayerControlEffectBase::DashAttack()
{
	Super::DashAttack();

	if (IsValid(InnerState))
	{
		InnerState->DashAttack();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlEffectBase::DashAttack : Invalid InnerState"));
	}
}

FEffectUIData UPlayerControlEffectBase::GetEffectUIData() const
{
	FString TagName = ControlEffectTag.GetTagName().ToString();
	FString LastSegment;
	TagName.Split(TEXT("."), nullptr, &LastSegment, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	return {
		ControlEffectTag,
		 LastSegment,
		EffectIcon,
		CachedDuration,
		DurationRemained
	};
}
