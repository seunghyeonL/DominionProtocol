// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashSkill.h"

#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"
#include "Util/GameTagList.h"

UPlayerDashSkill::UPlayerDashSkill()
{
	SkillTag = SkillTags::PlayerDash; 
	ControlEffectTag = EffectTags::UsingDash;
	DashMoveDirection = { 0.f, 0.f, 0.f };
	DashSpeed = 1000.f;
	DashDuration = 0.5f;
}

void UPlayerDashSkill::Execute()
{
	// Super::Execute();
	check(OwnerCharacter);

	if (Sounds.IsValidIndex(0))
	{
		UGameplayStatics::PlaySoundAtLocation(OwnerCharacter, Sounds[0], OwnerCharacter->GetActorLocation());
	}
	
	auto SkillComponent = Cast<USkillComponent>(GetOuter());
	check(SkillComponent);
	
	SetDashDirection();
	OwnerCharacter->GetWorldTimerManager().SetTimer(
		DurationTimer,
		SkillComponent,
		&USkillComponent::EndSkill,
		DashDuration,
		false
		);
}

void UPlayerDashSkill::SetDashDirection()
{
	check(IsValid(OwnerCharacter));
	
	auto ControlComponent = OwnerCharacter->FindComponentByClass<UPlayerControlComponent>();
	check(IsValid(ControlComponent));

	auto SkillComponent = Cast<USkillComponent>(GetOuter());

	auto LastMovementVector = ControlComponent->GetCurrentMovementVector();
		
	if (LastMovementVector.IsNearlyZero())
	{
		DashMoveDirection = -OwnerCharacter->GetActorForwardVector();
	}
	else
	{
		DashMoveDirection = LastMovementVector.GetSafeNormal();
	}

	SkillComponent->OnDashDirectionSet.ExecuteIfBound(DashMoveDirection);
}

void UPlayerDashSkill::Tick(float DeltaTime)
{
	check(OwnerCharacter);
	auto MovementComponent = OwnerCharacter->GetCharacterMovement();
	
	FVector Step = DashMoveDirection * DashSpeed * DeltaTime;
	FVector AdjustedStep = Step;

	// 1. 바닥에 닿아 있다면 바닥 Normal에 대해 투영
	if (MovementComponent->CurrentFloor.IsWalkableFloor())
	{
		FVector FloorNormal = MovementComponent->CurrentFloor.HitResult.Normal;
		FVector DashDirection = FVector::VectorPlaneProject(Step, FloorNormal).GetSafeNormal();
		AdjustedStep = DashDirection * DashSpeed * DeltaTime;
	}

	// 2. 벽에 닿아 있다면 벽 Normal에 대해 다시 투영
	// => 벽과 겹쳐있거나 옆에 비비고 있다면
	FHitResult WallHit;
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start + Step.GetSafeNormal() * 10.0f; // 가까운 벽 확인용
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	bool bHitWall = GetWorld()->SweepSingleByChannel(
		WallHit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility, // 필요 시 CustomChannel_Wall로 교체
		FCollisionShape::MakeCapsule(42.f, 96.f),
		Params
	);
	
	if (bHitWall)
	{
		const FVector WallNormal = WallHit.Normal;
		FVector DashDirection = FVector::VectorPlaneProject(AdjustedStep, WallNormal).GetSafeNormal();
		AdjustedStep = DashDirection * DashSpeed * DeltaTime;
	}
	// 여기까지 벽에서 미끄러지는 로직
	
	FHitResult Hit;
	OwnerCharacter->GetCharacterMovement()->SafeMoveUpdatedComponent(
		AdjustedStep,
		OwnerCharacter->GetActorRotation(),
		true,
		Hit
	);
}
