// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashSkill.h"

#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"
#include "Util/GameTagList.h"

UPlayerDashSkill::UPlayerDashSkill()
{
	SkillTag = SkillTags::PlayerDash; 
	ControlEffectTag = EffectTags::UsingDash;

	ControlComponent = nullptr;
	
	DashMoveDirection = { 0.f, 0.f, 0.f };
	
	MaxDashSpeed = 1500.f;
	DashDuration = 0.5f;
	
	DashMoveStart = 0.05f;
	DashMoveEnd = 0.45f;

	bIsMoving = false;

	TimeElapsed = 0.f;

	const int32 Idx0 = DashSpeedCurve.AddPoint(DashMoveStart, 0.f);
	DashSpeedCurve.Points[Idx0].InterpMode = CIM_CurveAuto;

	const int32 Idx1 = DashSpeedCurve.AddPoint((DashMoveStart + DashMoveEnd) / 2, MaxDashSpeed);
	DashSpeedCurve.Points[Idx1].InterpMode = CIM_CurveAuto;

	const int32 Idx2 = DashSpeedCurve.AddPoint(DashMoveEnd, 0.f);
	DashSpeedCurve.Points[Idx2].InterpMode = CIM_CurveAuto;
}

void UPlayerDashSkill::Initialize(ACharacter* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);

	ControlComponent = InOwnerCharacter->FindComponentByClass<UPlayerControlComponent>();
	check(ControlComponent);
}

void UPlayerDashSkill::Execute()
{
	// Super::Execute();
	check(IsValid(OwnerCharacter));
	TimeElapsed = 0.f;
	bIsMoving = false;

	// 실행중인 몽타주 중단
	// Dash는 몽타주를 안써서 수동으로 중단시켜줘야함.
	if (auto AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->Montage_IsPlaying(nullptr))
		{
			AnimInstance->Montage_Stop(0.1f);
		}
	}
	
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

	TimeElapsed += DeltaTime;

	// 선후딜 사이에만 이동
	if (TimeElapsed < DashMoveStart)
	{
		return;
	}

	if (TimeElapsed > DashMoveEnd)
	{
		// 이동 종료시 무적 해제
		if (bIsMoving)
		{
			bIsMoving = false;
			auto& ActiveControlEffectTags = ControlComponent->GetActiveControlEffectTags();
			ActiveControlEffectTags.RemoveTag(EffectTags::DashInvincible);
			OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
		}
		return;
	}

	// 이동 시작시 무적
	if (!bIsMoving)
	{
		bIsMoving = true;
		OwnerCharacter->GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
		auto& ActiveControlEffectTags = ControlComponent->GetActiveControlEffectTags();
		ActiveControlEffectTags.AddTag(EffectTags::DashInvincible);
	}
	
	auto MovementComponent = OwnerCharacter->GetCharacterMovement();
	
	FVector Step = DashMoveDirection * DashSpeedCurve.Eval(TimeElapsed) * DeltaTime;
	FVector AdjustedStep = Step;

	// 1. 바닥에 닿아 있다면 바닥 Normal에 대해 투영
	if (MovementComponent->CurrentFloor.IsWalkableFloor())
	{
		FVector FloorNormal = MovementComponent->CurrentFloor.HitResult.Normal;
		FVector DashDirection = FVector::VectorPlaneProject(Step, FloorNormal).GetSafeNormal();
		AdjustedStep = DashDirection * DashSpeedCurve.Eval(TimeElapsed) * DeltaTime;
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
		AdjustedStep = DashDirection * DashSpeedCurve.Eval(TimeElapsed) * DeltaTime;
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
