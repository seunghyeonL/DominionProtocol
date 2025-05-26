// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/LaserSkill.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "GameFramework/Character.h"
#include "Player/Damagable.h"
#include "Util/GameTagList.h"
#include "SkillObject/LaserActor.h"

ULaserSkill::ULaserSkill()
{
	SkillTag = SkillTags::LaserSkill;
}

void ULaserSkill::Execute()
{
	Super::Execute();

	check(OwnerCharacter);
}

void ULaserSkill::StartTrace()
{
	LaserActor = GetWorld()->SpawnActor<ALaserActor>(
		ALaserActor::StaticClass(),
		FVector::ZeroVector,
		FRotator::ZeroRotator
	);

	if (IsValid(LaserActor))
	{
		LaserActor->AttachToComponent(
			OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale
			, FName("LaserSocket")
		);

		LaserActor->SetOwnerCharacter(OwnerCharacter);

		LaserActor->Initialize();
	}
}

void ULaserSkill::StopTrace()
{
	check(LaserActor);

	LaserActor->Destroy();
	LaserActor = nullptr;
}

void ULaserSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return;
	}

	check(OwnerCharacter);

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
				float AttackPower = StatusComponent->GetStat(StatTags::AttackPower) * DeltaTime;

				AttackData.Damage = GetFinalAttackData(AttackPower);
			}
		}
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