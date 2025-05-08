#include "Components/SkillComponent/Skill.h"
#include "GameFramework/Character.h"
#include "CollisionQueryParams.h"
#include "Player/Damagable.h"

USkill::USkill()
{
	Stamina = 10.f;
	DamageCoefficient = 2.f;
}

void USkill::Excute(ACharacter* Owner) const
{
	check(AnimMontage);

	if (IsValid(Owner))
	{
		Owner->PlayAnimMontage(AnimMontage);
	}
}

// 애님 노티파이에서 실행
void USkill::AttackTrace(const ACharacter* Owner) const
{
	if (!IsValid(Owner))
	{
		return;
	}

	FVector ForwardVector = Owner->GetActorForwardVector();

	FVector Start = Owner->GetActorLocation() + ForwardVector * 150.f;
	FVector End = Start + ForwardVector * 150.f;

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
		100.f,
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

		// HitActor를 무언가로 캐스트해서 OnAttacked를 호출해야 함.
		if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			FAttackData AttackData(BaseAttackData);

			AttackData.Damage = GetFinalAttackData(BaseAttackData.Damage);

			IDamagable::Execute_OnAttacked(HitActor, BaseAttackData);
		}
	}
}

float USkill::GetStamina() const
{
	return Stamina;
}

float USkill::GetFinalAttackData(const float AttackPower) const
{
	return AttackPower * DamageCoefficient;
}


