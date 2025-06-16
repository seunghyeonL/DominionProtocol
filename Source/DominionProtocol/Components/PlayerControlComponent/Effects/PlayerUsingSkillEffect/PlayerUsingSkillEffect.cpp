// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUsingSkillEffect.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../BufferedInput/BufferedBaseAttack/BufferedBaseAttack.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Util/DebugHelper.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkillComponent/SkillComponent.h"

UPlayerUsingSkillEffect::UPlayerUsingSkillEffect()
{
	ControlEffectTag = EffectTags::UsingSkill;
	BufferedInputArray.Reserve(10);
	DoubleExecuteSkillEffectTags.AddTag(EffectTags::UsingTeleport);
}

void UPlayerUsingSkillEffect::Initialize()
{
	Super::Initialize();
	ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);
}

bool UPlayerUsingSkillEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	
	check(ControlComponent);
	check(OwnerCharacter);
	
	// Cashed Movement Vector
	const FVector& CurrentMovementVector = ControlComponent->GetCurrentMovementVector();
	
	if (!CurrentMovementVector.IsNearlyZero() && !ControlComponent->GetActiveControlEffectTags().HasTag(EffectTags::LockOn))
	{
		OwnerCharacter->SetActorRotation(CurrentMovementVector.Rotation());
		Debug::Print(FString::Printf(TEXT("UPlayerUsingSkillEffect::Activate : SetRotation : %f, %f, %f"), CurrentMovementVector.Rotation().Pitch, CurrentMovementVector.Rotation().Yaw, CurrentMovementVector.Rotation().Roll));
	}

	return true;
}

bool UPlayerUsingSkillEffect::Activate(float Duration)
{
	// Duration으로 사용하지 않음
	Activate();
	return true;
}

void UPlayerUsingSkillEffect::Deactivate()
{
	Super::Deactivate();
	
	SetControlEffectTag(EffectTags::UsingSkill);
	
	check(ControlComponent);
	
	for (int32 i = 0; i < BufferedInputArray.Num(); i++)
	{
		if (!BufferedInputArray[i]->IsExpired())
		{
			ControlComponent->SetValidBufferedInput(BufferedInputArray[i]);
			break;
		}
	}

	BufferedInputArray.Reset();
}

void UPlayerUsingSkillEffect::Move(const FInputActionValue& Value)
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::Move(Value);
		return;
	}
	
	check(OwnerCharacter);
	check(ControlComponent);
	
	if (auto Controller = OwnerCharacter->GetController())
	{
		FVector2D InputVector = Value.Get<FVector2D>();
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// final movement normal vector
		const FVector FinalMovementNormalVector = (ForwardDirection * InputVector.X  + RightDirection * InputVector.Y).GetSafeNormal();

		// caching final movement normal vector
		ControlComponent->SetCurrentMovementVector(FinalMovementNormalVector);
	}
}

void UPlayerUsingSkillEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerUsingSkillEffect::Dash()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::Dash();
	}
}

void UPlayerUsingSkillEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerUsingSkillEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerUsingSkillEffect::Parry()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::Parry();
	}
}

void UPlayerUsingSkillEffect::BaseAttack()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::BaseAttack();
		return;
	}

	check(ControlComponent);

	auto BufferedBaseAttack = NewObject<UBufferedBaseAttack>(ControlComponent);
	BufferedBaseAttack->SetTimer();
	BufferedInputArray.Add(BufferedBaseAttack);
}

void UPlayerUsingSkillEffect::WeaponSkill()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::WeaponSkill();
	}
}

void UPlayerUsingSkillEffect::MagicSkill()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::MagicSkill();
	}
}

void UPlayerUsingSkillEffect::Interact()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::Interact();
	}
}

void UPlayerUsingSkillEffect::LockOn()
{
	if (IsDoubleExecuteSkillEffect())
	{
		Super::LockOn();
	}
}

void UPlayerUsingSkillEffect::ConsumeItemAction_1()
{
	// Super::ConsumeItemAction_1();
}

void UPlayerUsingSkillEffect::ConsumeItemAction_2()
{
	// Super::ConsumeItemAction_2();
}

void UPlayerUsingSkillEffect::ConsumeItemAction_3()
{
	// Super::ConsumeItemAction_3();
}

void UPlayerUsingSkillEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

