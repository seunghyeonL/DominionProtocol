// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLockOnEffect.h"

#include <string>

#include "InputActionValue.h"
#include "VectorTypes.h"
#include "GameFramework/Character.h"
#include "Util/DebugHelper.h"

UPlayerLockOnEffect::UPlayerLockOnEffect()
{
	ControlEffectTag = EffectTags::LockOn;
}

void UPlayerLockOnEffect::Activate()
{
	Super::Activate();
}

void UPlayerLockOnEffect::Activate(float Duration)
{
	Super::Activate(Duration);
}

void UPlayerLockOnEffect::Deactivate()
{
	Super::Deactivate();
}

void UPlayerLockOnEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 타겟방향의 벡터 계산
	const FVector Target = FVector::ZeroVector;
	const FRotator NewControllerRotator = (Target - OwnerCharacter->GetActorLocation()).Rotation();
	const FRotator CurrentControlRotation = OwnerCharacter->GetControlRotation();
	const FRotator NewCharacterRotator = FRotator(0.f, NewControllerRotator.Yaw, NewControllerRotator.Roll);
	if (!OwnerCharacter) return;
	
	// 타겟을 바라보도록 회전 변경
	OwnerCharacter->SetActorRotation(NewCharacterRotator);
	OwnerCharacter->GetController()->SetControlRotation(FMath::RInterpTo(CurrentControlRotation, NewControllerRotator, DeltaTime, 10.0f));
}

void UPlayerLockOnEffect::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
}

void UPlayerLockOnEffect::Look(const FInputActionValue& Value)
{
	// DO NOT CHANGE ROTATION BY LOOK WHEN CAMERA LOCK ON 
}

void UPlayerLockOnEffect::Dash()
{
	Super::Dash();
}

void UPlayerLockOnEffect::SprintStart()
{
	Super::SprintStart();
}

void UPlayerLockOnEffect::SprintEnd()
{
	Super::SprintEnd();
}

void UPlayerLockOnEffect::Parry()
{
	Super::Parry();
}

void UPlayerLockOnEffect::BaseAttack()
{
	Super::BaseAttack();
}

void UPlayerLockOnEffect::WeaponSkill()
{
	Super::WeaponSkill();
}

void UPlayerLockOnEffect::MagicSkill()
{
	Super::MagicSkill();
}

void UPlayerLockOnEffect::Interact()
{
	Super::Interact();
}

void UPlayerLockOnEffect::LockOn()
{
	Super::LockOn();
}
