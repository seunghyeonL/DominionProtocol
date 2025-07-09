// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss3/Boss3Grab.h"
#include "AI/AICharacters/BossMonster/Boss3Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


UBoss3Grab::UBoss3Grab()
{
	SkillTag = SkillTags::Boss3Grab;
}

void UBoss3Grab::AttackTrace() const
{
	check(OwnerCharacter);

	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();

	FVector Start = OwnerCharacter->GetActorLocation() + ForwardVector * (AttackRadius + AttackForwardOffset);

	const USkeletalMeshSocket* TraceSocket = OwnerCharacter->GetMesh()->GetSocketByName("SpecialAttackSocket");

	// 소켓이 있다면 소켓에서 트레이스하기
	if (IsValid(TraceSocket))
	{
		Start = TraceSocket->GetSocketLocation(OwnerCharacter->GetMesh());
	}

	FVector End = Start;

	TArray<FHitResult> HitResults;

	// 트레이스 수행
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,										// 시작 위치
		End,										// 끝 위치
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,				// 충돌 채널
		FCollisionShape::MakeSphere(AttackRadius),	// 범위 설정 (구체 모양)
		QueryParams
	);


	if (!bHit)
	{
		return;
	}

	FAttackData AttackData;

	AttackData.Damage = GetFinalAttackData();

	AttackData.Instigator = OwnerCharacter;
	AttackData.Effects = Effects;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!IsValid(HitActor))
		{
			continue;
		}

		if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			if (CheckParry(HitActor))
			{
				return;
			}

			AttackData.LaunchVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

			AttackData.LaunchVector.Normalize();

			IDamagable::Execute_OnAttacked(HitActor, AttackData);

			SpawnHandMesh(Hit);
		}
	}
}

void UBoss3Grab::SpawnHandMesh(const FHitResult& Hit) const
{
	ACharacter* TargetCharacter = Cast<ACharacter>(Hit.GetActor());
	if (!IsValid(TargetCharacter)) return;

	TargetCharacter->GetCharacterMovement()->GravityScale = 0.0f;

	TargetCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 골렘 손 소켓에 부착
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	TargetCharacter->AttachToComponent(
		OwnerCharacter->GetMesh(),
		AttachRules,
		TEXT("SpecialAttackSocket")
	);

	if (ABoss3Enemy* Golem = Cast<ABoss3Enemy>(OwnerCharacter))
	{
		Golem->CapturedTarget = TargetCharacter;
	}
}