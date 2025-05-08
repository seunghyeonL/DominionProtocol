#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "GameFramework/Character.h"
#include "CollisionQueryParams.h"
#include "Player/Damagable.h"
#include "Components/StatusComponent/StatusComponentBase.h"
#include "Util/GameTagList.h"

UBaseSkill::UBaseSkill()
{
	Stamina = 10.f;
	DamageCoefficient = 2.f;

	AttackRadius = 100.0f;			// 공격 크기
	AttackForwardOffset = 50.0f;	// 캐릭터의 앞 위치
}

void UBaseSkill::Excute(ACharacter* Owner)
{
	check(AnimMontage);

	if (IsValid(Owner))
	{
		Owner->PlayAnimMontage(AnimMontage);

		UStatusComponentBase* StatusComponentBase = Owner->FindComponentByClass<UStatusComponentBase>();

		if (IsValid(StatusComponentBase))
		{
			int32 AttackPower = StatusComponentBase->GetStat(StatTags::AttackPower);

			BaseAttackData.Damage = GetFinalAttackData(AttackPower);
		}
	}
}

// 애님 노티파이에서 실행
void UBaseSkill::AttackTrace(const ACharacter* Owner) const
{
	if (!IsValid(Owner))
	{
		return;
	}

	FVector ForwardVector = Owner->GetActorForwardVector();

	FVector Start = Owner->GetActorLocation() + ForwardVector * (AttackRadius + AttackForwardOffset);
	FVector End = Start + ForwardVector * (AttackRadius + AttackForwardOffset);

	TArray<FHitResult> HitResults;

	// 트레이스 수행
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner); // 자신은 무시하도록 설정

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // 시작 위치
		End,                 // 끝 위치
		FQuat::Identity,     // 회전값 (회전 없이)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // 충돌 채널
		FCollisionShape::MakeSphere(100.f), // 범위 설정 (구체 모양)
		QueryParams
	);

	// 디버그용
	DrawDebugSphere(
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

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!IsValid(HitActor))
		{
			continue;
		}

		if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			IDamagable::Execute_OnAttacked(HitActor, BaseAttackData);
		}
	}
}

float UBaseSkill::GetStamina() const
{
	return Stamina;
}

float UBaseSkill::GetFinalAttackData(const float AttackPower) const
{
	return AttackPower * DamageCoefficient;
}