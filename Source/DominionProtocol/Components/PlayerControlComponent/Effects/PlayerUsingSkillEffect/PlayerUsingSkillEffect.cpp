// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUsingSkillEffect.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BufferedInput/BufferedBaseAttack/BufferedBaseAttack.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Util/DebugHelper.h"
#include "Components/CapsuleComponent.h"

UPlayerUsingSkillEffect::UPlayerUsingSkillEffect()
{
	ControlEffectTag = EffectTags::UsingSkill;
	BufferedInputArray.Reserve(10);
}

bool UPlayerUsingSkillEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);
	check(OwnerCharacter);

	if (ControlEffectTag == EffectTags::UsingDash)
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	}

	// Cashed Movement Vector
	const FVector& CurrentMovementVector = ControlComponent->GetCurrentMovementVector();

	if (!CurrentMovementVector.IsNearlyZero())
	{
		OwnerCharacter->SetActorRotation(CurrentMovementVector.Rotation());
		Debug::Print(FString::Printf(TEXT("UPlayerUsingSkillEffect::Activate : SetRotation : %f, %f, %f"), CurrentMovementVector.Rotation().Pitch, CurrentMovementVector.Rotation().Yaw, CurrentMovementVector.Rotation().Roll));
	}

	return true;
}

bool UPlayerUsingSkillEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}

	return true;
}

void UPlayerUsingSkillEffect::Deactivate()
{
	Super::Deactivate();
	if (ControlEffectTag == EffectTags::UsingDash)
	{
		OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	}
	
	SetControlEffectTag(EffectTags::UsingSkill);

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
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
	// Super::Move(Value);
	check(OwnerCharacter);

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
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
	// Super::Dash();
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
	Super::LockOn();
}

void UPlayerUsingSkillEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Debug::Print(TEXT("DashTick!"));

	// auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	// check(ControlComponent);
	//
	// AActor* LockOnTargetActor = ControlComponent->GetLockOnTargetActor();
	//
	// if (!IsValid(LockOnTargetActor))
	// {
	// 	return;
	// }

	// if (ControlEffectTag == EffectTags::UsingDash)
	// {
	// 	FVector LockOnTargetActorLocation = LockOnTargetActor->GetActorLocation();
	//
	// 	// 타겟방향의 벡터 계산
	// 	const FVector LockOnTargetActorEyeLocation = FVector(LockOnTargetActorLocation.X,LockOnTargetActorLocation.Y,150);
	// 	float ControllerLockOnHeight = 200;
	// 	const FRotator NewControllerRotator = (LockOnTargetActorEyeLocation - OwnerCharacter->GetActorLocation() - ControllerLockOnHeight * FVector::UpVector).Rotation();
	// 	const FRotator CurrentControlRotation = OwnerCharacter->GetControlRotation();
	// 	const FRotator NewCharacterRotator = FRotator(0.f, NewControllerRotator.Yaw, NewControllerRotator.Roll);
	// 	if (!OwnerCharacter) return;
	//
	// 	// 타겟을 바라보도록 회전 변경
	// 	OwnerCharacter->SetActorRotation(NewCharacterRotator);
	// 	OwnerCharacter->GetController()->SetControlRotation(FMath::RInterpTo(CurrentControlRotation, NewControllerRotator, DeltaTime, 10.0f));
	// }
	// else
	// {
		// Super::Tick(DeltaTime);
	// }
}

