// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiAnimInstance.h"
#include "DomiCharacter.h"
#include "../EffectReceivable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/DebugHelper.h"

UDomiAnimInstance::UDomiAnimInstance()
{
	Character = nullptr;
	MovementComponent = nullptr;
	bIsPlayer = false;
	bIsEffectReceivable = false;
	Velocity = FVector::ZeroVector;
	GroundSpeed = 0.f;
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
		NewTags = IEffectReceivable::Execute_GetActiveControlEffectTags(Character);
	}
	else
	{
		NewTags = IEffectReceivable::Execute_GetActiveStatusEffectTags(Character);
	}
	
	if ((ActiveAnimEffects != NewTags))
	{
		ActiveAnimEffects = MoveTemp(NewTags);
	}
	
	Velocity = MovementComponent->Velocity;
	GroundSpeed = GroundSpeed = Velocity.Size2D();

	// Set bShouldMove When Acceleration > 0 and GroundSpeed > 3.0(Little Threshold)
	bShouldMove = !MovementComponent->GetCurrentAcceleration().IsNearlyZero() && GroundSpeed > 3.0f;
	bShouldMove = GroundSpeed > 3.0f;

	// Set States From Character Movement Component
	bIsFalling = MovementComponent->IsFalling();
	// bIsCrouched = MovementComponent->IsCrouching();
	
}


