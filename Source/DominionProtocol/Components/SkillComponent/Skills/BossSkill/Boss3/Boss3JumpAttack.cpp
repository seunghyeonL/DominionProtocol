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
				//JumpToTarget();
			}
		}
	}

	LaunchParabolicAtTarget(TargetActor1, 1.45f);

	Super::Execute();
}

//void UBoss3JumpAttack::JumpToTarget()
//{
//	if (!IsValid(TargetActor1)) return;
//
//	FVector StartLocation = OwnerCharacter->GetActorLocation();
//	FVector TargetLocation = TargetActor1->GetActorLocation();
//
//	// 단순 방향 + 거리 기반 점프 벡터 (보정 가능)
//	FVector LaunchVelocity = (TargetLocation - StartLocation).GetSafeNormal() * 10000.f;
//	LaunchVelocity.Z = 1500.f; // 위로 띄우는 힘
//
//	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true);
//}

void UBoss3JumpAttack::LaunchParabolicAtTarget(AActor* TargetActor, float TimeToReach)
{
	if (!IsValid(OwnerCharacter) || !IsValid(TargetActor)) return;

	const FVector StartLocation = OwnerCharacter->GetActorLocation();
	const FVector EndLocation = TargetActor->GetActorLocation();

	const float Gravity = FMath::Abs(GetWorld()->GetGravityZ());

	FVector Delta = EndLocation - StartLocation;
	FVector DeltaXY = FVector(Delta.X, Delta.Y, 0.f);
	float HorizontalDistance = DeltaXY.Size();

	float DeltaZ = Delta.Z;

	FVector HorizontalDir = DeltaXY.GetSafeNormal();
	float HorizontalSpeed = HorizontalDistance / TimeToReach;

	float VerticalSpeed = (DeltaZ + 0.5f * Gravity * FMath::Square(TimeToReach)) / TimeToReach;

	FVector LaunchVelocity = HorizontalDir * HorizontalSpeed;
	LaunchVelocity.Z = VerticalSpeed;

	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true);
}
