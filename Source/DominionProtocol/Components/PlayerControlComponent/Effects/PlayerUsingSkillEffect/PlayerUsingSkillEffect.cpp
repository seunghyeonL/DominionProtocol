// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUsingSkillEffect.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BufferedInput/BufferedBaseAttack/BufferedBaseAttack.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Util/DebugHelper.h"

UPlayerUsingSkillEffect::UPlayerUsingSkillEffect()
{
	ControlEffectTag = EffectTags::UsingSkill;
}

void UPlayerUsingSkillEffect::Activate()
{
	Super::Activate();
}

void UPlayerUsingSkillEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerUsingSkillEffect::Deactivate()
{
	Super::Deactivate();
	SetControlEffectTag(EffectTags::UsingSkill);

	UBaseBufferedInput* ValidBufferedInput = nullptr;
	for (int32 i = 0; i < BufferedInputArray.Num(); i++)
	{
		if (!BufferedInputArray[i]->IsExpired())
		{
			ValidBufferedInput = BufferedInputArray[i];
			break;
		}
	}
	BufferedInputArray.Empty();
	
	if (IsValid(ValidBufferedInput))
	{
		ValidBufferedInput->Operate();
	}
}

void UPlayerUsingSkillEffect::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
	check(OwnerCharacter);
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
		if (auto ControlComponentUser = Cast<IControlComponentUser>(OwnerCharacter))
		{
			ControlComponentUser->SetLastMovementVector(FinalMovementNormalVector);
		}
	}
}

void UPlayerUsingSkillEffect::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void UPlayerUsingSkillEffect::Dash()
{
	// Super::Dash();
}

void UPlayerUsingSkillEffect::SprintStart()
{
	// Super::SprintStart();
}

void UPlayerUsingSkillEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerUsingSkillEffect::Parry()
{
	// Super::Parry();
}

void UPlayerUsingSkillEffect::BaseAttack()
{
	// Super::BaseAttack();
	if (auto ControlComponent = GetOuter())
	{
		auto BufferedBaseAttack = NewObject<UBufferedBaseAttack>(ControlComponent);
		BufferedBaseAttack->SetTimer();
		BufferedInputArray.Add(BufferedBaseAttack);
	}
}

void UPlayerUsingSkillEffect::WeaponSkill()
{
	// Super::WeaponSkill();
}

void UPlayerUsingSkillEffect::MagicSkill()
{
	// Super::MagicSkill();
}

void UPlayerUsingSkillEffect::Interact()
{
	// Super::Interact();
}

void UPlayerUsingSkillEffect::LockOn()
{
	// Super::LockOn();
}

void UPlayerUsingSkillEffect::Tick(float DeltaTime)
{
	// Super::Tick(DeltaTime);
	// Debug::Print(TEXT("DashTick!"));
}

