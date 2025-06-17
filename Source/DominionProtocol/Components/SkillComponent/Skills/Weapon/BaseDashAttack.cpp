// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDashAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBaseDashAttack::UBaseDashAttack()
{
	TimeElapsed = 0.f;
	MoveSpeed = 900.f;
	MoveDuration = 0.4f;
}

void UBaseDashAttack::Execute()
{
	Super::Execute();
	
	TimeElapsed = 0.f;
}

void UBaseDashAttack::Tick(float DeltaTime)
{
	check(OwnerCharacter);

	TimeElapsed += DeltaTime;
	if (TimeElapsed > MoveDuration)
	{
		return;
	}

	auto MovementComponent = OwnerCharacter->GetCharacterMovement();
	FVector MoveDirection = OwnerCharacter->GetActorForwardVector();
	
	FVector Step = MoveDirection * MoveSpeed * DeltaTime;
	FVector AdjustedStep = Step;

	// 1. 바닥에 닿아 있다면 바닥 Normal에 대해 투영
	if (MovementComponent->CurrentFloor.IsWalkableFloor())
	{
		FVector FloorNormal = MovementComponent->CurrentFloor.HitResult.Normal;
		FVector DashDirection = FVector::VectorPlaneProject(Step, FloorNormal).GetSafeNormal();
		AdjustedStep = DashDirection * MoveSpeed * DeltaTime;
	}
	
	FHitResult Hit;
	OwnerCharacter->GetCharacterMovement()->SafeMoveUpdatedComponent(
		AdjustedStep,
		OwnerCharacter->GetActorRotation(),
		true,
		Hit
	);
}

