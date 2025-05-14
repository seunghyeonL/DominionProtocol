#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "GameFramework/Character.h"
#include "CollisionQueryParams.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentUser.h"
#include "Player/Damagable.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Util/GameTagList.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/Skills/SkillData.h"

UBaseSkill::UBaseSkill()
{
	ControlEffectTag = EffectTags::UsingSkill;
	OwnerCharacter = nullptr;
}

void UBaseSkill::Initialize(ACharacter* InOwnerCharacter)
{
	UWorld* World = GetWorld();
	OwnerCharacter = InOwnerCharacter;

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
			{
				AnimMontage = SkillData->AnimMontage;
				Sound = SkillData->Sound;
				Particle = SkillData->Particle;
				Stamina = SkillData->Stamina;
				AttackRadius = SkillData->AttackRadius;
				AttackForwardOffset = SkillData->AttackForwardOffset;
				DamageCoefficient = SkillData->DamageCoefficient;
				Effects = SkillData->Effects;
			}
			else
			{
				Debug::PrintError(TEXT("UBaseSkill::Initialize : Invalid SkillData."));
			}
		}
		else
		{
			Debug::PrintError(TEXT("UBaseSkill::Initialize : Invalid BaseGameState."));
		}
	}
}

void UBaseSkill::Execute()
{
	check(AnimMontage);

	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->PlayAnimMontage(AnimMontage);
	}
}

// 애님 노티파이에서 실행
void UBaseSkill::AttackTrace() const
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();

	FVector Start = OwnerCharacter->GetActorLocation() + ForwardVector * (AttackRadius + AttackForwardOffset);
	FVector End = Start + ForwardVector * (AttackRadius + AttackForwardOffset);

	TArray<FHitResult> HitResults;

	// 트레이스 수행
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter); // 자신은 무시하도록 설정

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // 시작 위치
		End,                 // 끝 위치
		FQuat::Identity,     // 회전값 (회전 없이)
		ECollisionChannel::ECC_Pawn, // 충돌 채널
		FCollisionShape::MakeSphere(AttackRadius), // 범위 설정 (구체 모양)
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
				int32 AttackPower = StatusComponent->GetStat(StatTags::AttackPower);

				AttackData.Damage = GetFinalAttackData(AttackPower);
			}

			AttackData.Instigator = OwnerCharacter;
			AttackData.Effects = Effects;
		}
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		USkillComponent* HitActorSkillComponent = HitActor->FindComponentByClass<USkillComponent>();

		if (!IsValid(HitActor))
		{
			continue;
		}

		if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
		{
			AttackData.LaunchVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

			AttackData.LaunchVector.Normalize();

			// 피격자가 실행 중이던 스킬 중단
			HitActorSkillComponent->StopSkill();
			Debug::Print(FString::Printf(TEXT("%s :: Skill is canceled."), *HitActor->GetName()));

			IDamagable::Execute_OnAttacked(HitActor, AttackData);
		}
	}
}

float UBaseSkill::GetStamina() const
{
	return Stamina;
}

void UBaseSkill::Tick(float DeltaTime)
{
	if(!bIsExecuting)
	{
		return;
	}
}

TObjectPtr<UAnimMontage> UBaseSkill::GetAnimMontage() const
{
	return AnimMontage;
}

float UBaseSkill::GetFinalAttackData(const float AttackPower) const
{
	return AttackPower * DamageCoefficient;
}
