// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/MagicSkill/MagicLaserSkill.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "GameFramework/Character.h"
#include "Player/Damagable.h"
#include "Util/GameTagList.h"
#include "Components/SkillComponent/Skills/SkillObject/LaserActor.h"

UMagicLaserSkill::UMagicLaserSkill()
{
	SkillTag = SkillTags::MagicLaserSkill;
}

void UMagicLaserSkill::Execute()
{
	Super::Execute();

	check(OwnerCharacter);
}

void UMagicLaserSkill::StartTrace(const FGameplayTagContainer& TagContainer)
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

void UMagicLaserSkill::StopTrace(const FGameplayTagContainer& TagContainer)
{
	check(LaserActor);

	LaserActor->Destroy();
	LaserActor = nullptr;
}

void UMagicLaserSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return;
	}

	check(OwnerCharacter);

	FAttackData AttackData;

	// UWorld* World = GetWorld();

	// if (IsValid(World))
	// {
	// 	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	//
	// 	if (IsValid(BaseGameState))
	// 	{
	// 		UStatusComponent* StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	//
	// 		if (IsValid(StatusComponent))
	// 		{
	// 			float AttackPower = StatusComponent->GetStat(StatTags::AttackPower) * DeltaTime;
	//
	// 			AttackData.Damage = GetFinalAttackData();
	// 		}
	// 	}
	// 	
	// }

	AttackData.Damage = GetFinalAttackData() * DeltaTime;
	
	AttackData.Instigator = OwnerCharacter;
	AttackData.Effects = Effects;

	if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
	{
		AttackData.LaunchVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

		AttackData.LaunchVector.Normalize();

		IDamagable::Execute_OnAttacked(HitActor, AttackData);
	}
}
