// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss3/Boss3JumpAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBoss3JumpAttack::UBoss3JumpAttack()
{
	SkillTag = SkillTags::Boss3JumpAttack;
}

void UBoss3JumpAttack::Execute()
{
	if (AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
			if (IsValid(Target))
			{
				TargetActor1 = Target;
				JumpToTarget();
			}
		}
	}

	//LaunchParabolicAtTarget(TargetActor1, 1.45f);

	Super::Execute();
}

void UBoss3JumpAttack::JumpToTarget()
{
	if (!IsValid(TargetActor1)) return;

	FVector StartLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = TargetActor1->GetActorLocation();

	// 단순 방향 + 거리 기반 점프 벡터 (보정 가능)
	FVector LaunchVelocity = (TargetLocation - StartLocation).GetSafeNormal() * 10000.f;
	LaunchVelocity.Z = 1500.f; // 위로 띄우는 힘

	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true);
}

void UBoss3JumpAttack::LaunchParabolicAtTarget(AActor* TargetActor, float TimeToReach)
{
	if (!IsValid(OwnerCharacter) || !IsValid(TargetActor)) return;

	const FVector StartLocation = OwnerCharacter->GetActorLocation();
	const FVector EndLocation = TargetActor->GetActorLocation();

	// 중력 가속도 (언리얼에선 음수, 기본적으로 -980)
	const float Gravity = FMath::Abs(GetWorld()->GetGravityZ());

	// XY 수평 방향만 따로 계산
	FVector Delta = EndLocation - StartLocation;
	FVector DeltaXY = FVector(Delta.X, Delta.Y, 0.f);
	float HorizontalDistance = DeltaXY.Size();

	// Z 높이 차이
	float DeltaZ = Delta.Z;

	// 수평 속도 계산
	FVector HorizontalDir = DeltaXY.GetSafeNormal();
	float HorizontalSpeed = HorizontalDistance / TimeToReach;

	// 수직 속도 계산
	float VerticalSpeed = (DeltaZ + 0.5f * Gravity * FMath::Square(TimeToReach)) / TimeToReach;

	// 최종 속도 벡터
	FVector LaunchVelocity = HorizontalDir * HorizontalSpeed;
	LaunchVelocity.Z = VerticalSpeed;

	// 디버그용 방향 표시
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.f, 0, 3.f);
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, StartLocation + LaunchVelocity, 100.f, FColor::Green, false, 2.f, 0, 3.f);

	// 점프 실행
	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true);
}
