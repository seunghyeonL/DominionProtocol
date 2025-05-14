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

void UPlayerLockOnEffect::Move(const FInputActionValue& Value)
{
	// const FVector2D MovementVector = Value.Get<FVector2D>();
	// const FVector Target = FVector::ZeroVector;
	// // 타겟과의 거리(반지름)
	// float Radius = UE::Geometry::Distance(OwnerCharacter->GetActorLocation(), Target);
	
	// Super::Move 호출
	Super::Move(Value);
	
	// // 타겟방향의 벡터 계산
	// FRotator NewRotator = (Target - OwnerCharacter->GetActorLocation()).Rotation();
	// if (!OwnerCharacter) return;
	//
	// // 타겟을 바라보도록 회전 변경
	// // OwnerCharacter->SetActorRotation(NewRotator);
	// OwnerCharacter->GetController()->SetControlRotation(NewRotator);
}

void UPlayerLockOnEffect::Look(const FInputActionValue& Value)
{
	// DO NOT CHANGE ROTATION BY LOOK WHEN CAMERA LOCK ON 
}

void UPlayerLockOnEffect::Dash()
{
	Super::Dash();
}

void UPlayerLockOnEffect::Sprint()
{
	Super::Sprint();
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

void UPlayerLockOnEffect::RockOn()
{
	Super::RockOn();
}
