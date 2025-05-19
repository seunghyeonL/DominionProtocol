// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/LaserSkill.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "GameFramework/Character.h"
#include "Player/Damagable.h"
#include "Util/GameTagList.h"

ULaserSkill::ULaserSkill()
{
	SkillTag = SkillTags::LaserSkill;
}

void ULaserSkill::Initialize(ACharacter* Owner)
{
	Super::Initialize(Owner);

	UMnhTracerComponent* TracerComponent = OwnerCharacter->FindComponentByClass<UMnhTracerComponent>();

	if (!IsValid(TracerComponent))
	{
		return;
	}

	// TraceBox
	// 컴포넌트 생성
	CapsuleComponent = NewObject<UMnhCapsuleComponent>(OwnerCharacter, TEXT("MagicTraceBox"));

	// 액터의 메시에 부착
	CapsuleComponent->AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	
	// 컴포넌트 등록 (필수!)
	CapsuleComponent->RegisterComponent();

	FMnhTracerConfig TracerConfig;

	TracerConfig.TracerTag = SkillTag;
	TracerConfig.TraceSource = EMnhTraceSource::AnimNotify;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;
	TracerConfig.DrawDebugType = EDrawDebugTrace::ForOneFrame;

	TracerComponent->TracerConfigs.Add(TracerConfig);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(SkillTag);

	TracerComponent->InitializeTracers(TagContainer, CapsuleComponent);

	//TracerComponent->FilterType = EMnhFilterType::None;
}

void ULaserSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return;
	}

	check(OwnerCharacter)

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

void ULaserSkill::BeginDestroy()
{
	if (IsValid(CapsuleComponent))
	{
		CapsuleComponent->DestroyComponent();
		CapsuleComponent = nullptr;
	}

	Super::BeginDestroy();
}
