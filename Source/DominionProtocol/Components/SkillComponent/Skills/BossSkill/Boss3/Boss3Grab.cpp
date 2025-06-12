// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss3/Boss3Grab.h"
#include "AI/AICharacters/BossMonster/Boss3Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

UBoss3Grab::UBoss3Grab()
{
	SkillTag = SkillTags::Boss3Grab;
}

void UBoss3Grab::AttackTrace() const
{
	check(OwnerCharacter);
	
	FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * (AttackRadius + AttackForwardOffset);

	const USkeletalMeshSocket* TraceSocket = OwnerCharacter->GetMesh()->GetSocketByName("GrabSocket");
	if (IsValid(TraceSocket))
	{
		Start = TraceSocket->GetSocketLocation(OwnerCharacter->GetMesh());
	}

	FVector End = Start;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		QueryParams
	);

	// 디버그 시각화
	DrawDebugSphere(GetWorld(), Start, AttackRadius, 12, FColor::Red, false, 2.0f);

	if (!bHit) return;

	// 공격 데이터 구성 (원한다면)
	FAttackData AttackData;
	AttackData.Damage = 0.f; // 피해는 없음
	AttackData.Instigator = OwnerCharacter;
	AttackData.Effects = Effects;

	for (const FHitResult& Hit : HitResults)
	{
		ACharacter* TargetCharacter = Cast<ACharacter>(Hit.GetActor());
		if (!IsValid(TargetCharacter)) continue;

		// 파리 성공 체크 등은 필요 없다면 생략

		// 손에 붙이기
		TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
		TargetCharacter->AttachToComponent(
			OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("GrabSocket")
		);
		TargetCharacter->SetActorEnableCollision(false);

		// 보스가 기억하도록 저장
		if (ABoss3Enemy* Boss = Cast<ABoss3Enemy>(OwnerCharacter))
		{
			Boss->CapturedTarget = TargetCharacter;
		}

		break; // 첫 대상만 처리
	}
}
