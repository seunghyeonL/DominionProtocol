// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss1/Boss1JumpAttack.h"
#include "Util/GameTagList.h"
#include "GameFramework/Character.h"
#include "Player/Damagable.h"

UBoss1JumpAttack::UBoss1JumpAttack()
{
	SkillTag = SkillTags::Boss1JumpAttack;

	AnimPlayRate = 0.3f;
}

void UBoss1JumpAttack::Execute()
{
	FVector Start = TargetLocation = OwnerCharacter->GetActorLocation();
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
		FCollisionShape::MakeSphere(3000.f),	// 범위 설정 (구체 모양)
		QueryParams
	);

	// 디버그용
	DrawDebugSphere(
		GetWorld(),
		Start,
		3000.f,
		12,
		FColor::Green,
		false,              // 지속 여부 (false면 일정 시간 후 사라짐)
		2.0f,               // 지속 시간 (초)
		0,
		1.0f
	);

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!IsValid(HitActor))
		{
			continue;
		}

		if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			TargetLocation = HitActor->GetActorLocation();
			break;
		}
	}

	Super::Execute();
}

void UBoss1JumpAttack::MoveToLocation()
{
	FVector Destination = TargetLocation;

	Destination.Z = OwnerCharacter->GetActorLocation().Z;

	OwnerCharacter->SetActorLocation(Destination);
}
