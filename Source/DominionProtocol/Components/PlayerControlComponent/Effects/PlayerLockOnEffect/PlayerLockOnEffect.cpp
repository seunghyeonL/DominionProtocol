// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLockOnEffect.h"

#include <string>

#include "InputActionValue.h"
#include "LandscapeGizmoActiveActor.h"
#include "VectorTypes.h"
#include "AI/AIControllers/BaseAIController.h"
#include "GameFramework/Character.h"
#include "Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"

UPlayerLockOnEffect::UPlayerLockOnEffect()
{
	ControlEffectTag = EffectTags::LockOn;
}

bool UPlayerLockOnEffect::Activate()
{
	if (!Super::Activate())
	{
		return false;
	}
	return true;
}

bool UPlayerLockOnEffect::Activate(float Duration)
{
	if (!Super::Activate(Duration))
	{
		return false;
	}
	return true;
}

void UPlayerLockOnEffect::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}
	
	Super::Deactivate();
}

void UPlayerLockOnEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(IsValid(OwnerCharacter));

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	check(ControlComponent);

	AActor* LockOnTargetActor = ControlComponent->GetLockOnTargetActor();
	auto StatusComponenetUser = Cast<IStatusComponentUser>(LockOnTargetActor);
	check(StatusComponenetUser);
	if (StatusComponenetUser->GetActiveStatusEffectTags().HasTagExact(EffectTags::Death))
	{
		LockOnTargetActor = nullptr;
		Deactivate();
		ControlComponent->LockOn();
		return;
	}
	
	FVector LockOnTargetActorLocation = LockOnTargetActor->GetActorLocation();

	auto Controller = OwnerCharacter->GetController();
	check(Controller);
	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	// 타겟방향의 벡터 계산
	FRotator NewControllerRotator = (LockOnTargetActorLocation - ViewLocation).Rotation();
	NewControllerRotator.Pitch = FMath::Min(NewControllerRotator.Pitch, -15.0f);
	
	const FRotator CurrentControlRotation = OwnerCharacter->GetControlRotation();
	const FRotator NewCharacterRotator = FRotator(0.f, NewControllerRotator.Yaw, NewControllerRotator.Roll);
	
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
