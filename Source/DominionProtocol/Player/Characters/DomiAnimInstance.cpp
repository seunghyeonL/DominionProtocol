// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiAnimInstance.h"
#include "DomiCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"

UDomiAnimInstance::UDomiAnimInstance()
{
	Character = nullptr;
	MovementComponent = nullptr;
	bIsPlayer = false;
	Velocity = FVector::ZeroVector;
	GroundSpeed = 0.f;
	LockOnAngle = 0.f;
	bShouldMove = false;
	bIsFalling = false;
}

void UDomiAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Character) || !IsValid(MovementComponent))
	{
		// Debug::PrintError(TEXT("UDomiAnimInstance::NativeUpdateAnimation : Invalid Character."));
		return;
	}

	FGameplayTagContainer NewTags;
	
	if (bIsPlayer)
	{
		NewTags = Cast<IControlComponentUser>(Character)->GetActiveControlEffectTags();
	}
	else
	{
		NewTags = Cast<IStatusComponentUser>(Character)->GetActiveStatusEffectTags();
	}
	
	if ((ActiveAnimEffects != NewTags))
	{
		ActiveAnimEffects = MoveTemp(NewTags);
	}
	
	Velocity = MovementComponent->Velocity;
	GroundSpeed = GroundSpeed = Velocity.Size2D();

	FVector ForwardVector = Character->GetActorForwardVector();
	FVector MoveDirection = Velocity.GetSafeNormal();

	float Dot = FVector::DotProduct(ForwardVector, MoveDirection);
	float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(Dot));

	FVector Cross = FVector::CrossProduct(ForwardVector, MoveDirection);
	
	// + -> LockOnTarget is at RightSide, - : -> LockOnTarget is at LeftSide
	LockOnAngle = (Cross.Z > 0) ? AngleDegrees : -AngleDegrees;

	// Set bShouldMove When Acceleration > 0 and GroundSpeed > 3.0(Little Threshold)
	bShouldMove = bIsPlayer ? !MovementComponent->GetCurrentAcceleration().IsNearlyZero() && GroundSpeed > 3.0f : GroundSpeed > 3.0f;

	// Set States From Character Movement Component
	bIsFalling = MovementComponent->IsFalling();
	// bIsCrouched = MovementComponent->IsCrouching();
	
}


