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

	LaserActor = GetWorld()->SpawnActor<ALaserActor>(
		ALaserActor::StaticClass(),
		FVector::ZeroVector,
		FRotator::ZeroRotator
	);

	// AttachToComponent으로 부착
	LaserActor->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale
		, FName("headSocket")
	);

	LaserActor->SetOwnerCharacter(OwnerCharacter);

	LaserActor->Initialize();

	LaserActor->SetActorRelativeRotation(FRotator(0.f, 90.f, 0.f));
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