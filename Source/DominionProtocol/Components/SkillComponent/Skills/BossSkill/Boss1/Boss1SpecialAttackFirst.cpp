// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss1/Boss1SpecialAttackFirst.h"
#include "Util/GameTagList.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Player/Damagable.h"
#include "Engine/SkeletalMeshSocket.h"

UBoss1SpecialAttackFirst::UBoss1SpecialAttackFirst()
{
	SkillTag = SkillTags::Boss1SpecialAttack1;
	AnimPlayRate = 0.15f;
}

void UBoss1SpecialAttackFirst::AttackTrace() const
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
	
	// 디버그용
	Debug::DrawSphere(
		GetWorld(),
		Start,
		AttackRadius,
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

	FAttackData AttackData;

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			UStatusComponent* StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();

			if (IsValid(StatusComponent))
			{
				float AttackPower = StatusComponent->GetStat(StatTags::AttackPower);

				AttackData.Damage = GetFinalAttackData(AttackPower);
			}
		}
	}

	AttackData.Instigator = OwnerCharacter;
	AttackData.Effects = Effects;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		// USkillComponent* HitActorSkillComponent = HitActor->FindComponentByClass<USkillComponent>();

		if (!IsValid(HitActor))
		{
			continue;
		}

		if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			AttackData.LaunchVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

			AttackData.LaunchVector.Normalize();

			// 피격자가 실행 중이던 스킬 중단
			// HitActorSkillComponent->StopSkill();
			// Debug::Print(FString::Printf(TEXT("%s :: Skill is canceled."), *HitActor->GetName()));

			IDamagable::Execute_OnAttacked(HitActor, AttackData);

			SpawnHandMesh(Hit);
		}
	}
}
