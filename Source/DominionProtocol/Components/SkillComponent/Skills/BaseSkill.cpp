#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "GameFramework/Character.h"
#include "CollisionQueryParams.h"
#include "Player/Damagable.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Util/GameTagList.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/Skills/SkillData.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "Util/DebugHelper.h"
#include "Interface/Parryable.h"

UBaseSkill::UBaseSkill()
{
	ControlEffectTag = EffectTags::UsingSkill;

	AnimPlayRate = 1.0f;
}

void UBaseSkill::Initialize(ACharacter* InOwnerCharacter)
{
	if (IsValid(InOwnerCharacter))
	{
		OwnerCharacter = InOwnerCharacter;
	}

	UWorld* World = GetWorld();

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
				CurvedProjectileClass = SkillData->CurvedProjectileClass;
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
	check(IsValid(AnimMontage));
	check(IsValid(OwnerCharacter));

	OwnerCharacter->PlayAnimMontage(AnimMontage, AnimPlayRate);
}

// 애님 노티파이에서 실행
void UBaseSkill::AttackTrace() const
{
	check(OwnerCharacter);

	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();

	FVector Start = OwnerCharacter->GetActorLocation() + ForwardVector * (AttackRadius + AttackForwardOffset);
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
		ECollisionChannel::ECC_Pawn,    			// 충돌 채널
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

	for (const FHitResult& HitResult : HitResults)
	{
		ApplyAttackToHitActor(HitResult, 0.f);
	}
}

void UBaseSkill::StartTrace()
{
	check(OwnerCharacter);

	UMnhTracerComponent* MnhTracerComponent = OwnerCharacter->FindComponentByClass<UMnhTracerComponent>();

	if (IsValid(MnhTracerComponent))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(ItemTags::BasicWeapon);

		MnhTracerComponent->StartTracers(TagContainer);
	}
}

void UBaseSkill::StopTrace()
{
	check(OwnerCharacter);

	UMnhTracerComponent* MnhTracerComponent = OwnerCharacter->FindComponentByClass<UMnhTracerComponent>();

	if (IsValid(MnhTracerComponent))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(ItemTags::BasicWeapon);

		MnhTracerComponent->StopTracers(TagContainer);
	}
}

void UBaseSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return;
	}

	check(OwnerCharacter)

	// check Parry
	if (CheckParry(HitActor))
	{
		return;
	}
	
	FAttackData AttackData;
	
	UStatusComponent* StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();

	if (IsValid(StatusComponent))
	{
		float AttackPower = StatusComponent->GetStat(StatTags::AttackPower);

		AttackData.Damage = GetFinalAttackData(AttackPower);
	}
	
	AttackData.Instigator = OwnerCharacter;
	AttackData.Effects = Effects;

	if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
	{
		AttackData.LaunchVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

		AttackData.LaunchVector.Normalize();
		
		IDamagable::Execute_OnAttacked(HitActor, AttackData);
	}
}

float UBaseSkill::GetStamina() const
{
	return Stamina;
}

void UBaseSkill::Tick(float DeltaTime)
{
}

float UBaseSkill::GetFinalAttackData(const float AttackPower) const
{
	return AttackPower * DamageCoefficient;
}

bool UBaseSkill::CheckParry(AActor* HitActor) const
{
	FVector AttackDirectionVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation() ;
	FVector TargetForwardVector = HitActor->GetActorForwardVector();

	float RadianAngle = FMath::Acos(FVector::DotProduct(TargetForwardVector, -AttackDirectionVector));
	float DegreeAngle = FMath::RadiansToDegrees(RadianAngle);
	if (DegreeAngle > 45.f)
	{
		return false;
	}
	
	auto ParryableTarget = Cast<IParryable>(HitActor);
    if (!ParryableTarget || !ParryableTarget->IsParryingCond())
    {
	    return false;
    }

	auto ParryableOwner = Cast<IParryable>(OwnerCharacter);
	if (!ParryableOwner)
	{
		return false;
	}
	
	ParryableOwner->OnParried();
	return true;
}
