// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/Boss4AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Components/StatusComponent/StatusComponent.h"

ABoss4AIController::ABoss4AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultBaseAttackWeight = 4;
	DefaultFlameStrikeWeight = 4;
	DefaultInfernoSpikeWeight = 4;
	DefaultHellfireFallWeight = 0;
	DefaultBlazeSphereWeight = 2;
	DefaultWeaponAttack1Weight = 2;
	DefaultBackDashWeight = 3;
	DefaultTeleportWeight = 0;

	BaseAttackWeightIncrement = 2;
	FlameStrikeWeightIncrement = 2;
	InfernoSpikeWeightIncrement = 2;
	HellfireFallWeightIncrement = 1;
	BlazeSphereWeightIncrement = 1;
	WeaponAttack1WeightIncrement = 1;
	BackDashWeightIncrement = 1;
	TeleportWeightIncrement = 1;

	HellfireFallCoolDown = 60.f;
	BlazeSphereCoolDown = 20.f;
	WeaponAttack1CoolDown = 20.f;
	TeleportCoolDown = 20.f;

	bIsActiveHellfireFall = true;
	bIsActiveBlazeSphere = true;
	bIsActiveWeaponAttack1 = true;
	bIsActiveTeleport = true;

	bIsClone = false;
	bIsActiveSpawnClone = false;

	CachedMaxHealth = 0.f;
}

void ABoss4AIController::BeginPlay()
{
	Super::BeginPlay();

	CurrentBaseAttackWeight = DefaultBaseAttackWeight;
	CurrentFlameStrikeWeight = DefaultFlameStrikeWeight;
	CurrentInfernoSpikeWeight = DefaultInfernoSpikeWeight;
	CurrentHellfireFallWeight = DefaultHellfireFallWeight;
	CurrentBlazeSphereWeight = DefaultBlazeSphereWeight;
	CurrentWeaponAttack1Weight = DefaultWeaponAttack1Weight;
	CurrentBackDashWeight = DefaultBackDashWeight;
	CurrentTeleportWeight = DefaultTeleportWeight;

	APawn* Boss4Pawn = GetPawn();

	if (IsValid(Boss4Pawn))
	{
		ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(Boss4Pawn);

		CachedStatusComponent = Boss4->GetStatusComponent();
		CachedMaxHealth = CachedStatusComponent->GetStat(StatTags::MaxHealth);
	}
}

void ABoss4AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsClone || bIsActiveSpawnClone)
	{
		return;
	}

	// Boss4 스폰 시점에 따른 처리
	if (!IsValid(CachedStatusComponent) || FMath::IsNearlyZero(CachedMaxHealth))
	{
		APawn* Boss4Pawn = GetPawn();

		if (IsValid(Boss4Pawn))
		{
			ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(Boss4Pawn);

			CachedStatusComponent = Boss4->GetStatusComponent();
			CachedMaxHealth = CachedStatusComponent->GetStat(StatTags::MaxHealth);
		}
	}

	// 위에서 처리했음에도 비어있다면 예외처리
	if (!IsValid(CachedStatusComponent) || FMath::IsNearlyZero(CachedMaxHealth))
	{
		return;
	}

	if (!IsValid(Blackboard))
	{
		return;
	}

	float CurrentHealth = CachedStatusComponent->GetStat(StatTags::Health);
	float CurrentHealthPercent = CurrentHealth / CachedMaxHealth;

	if (CurrentHealthPercent > 0.5f)
	{
		return;
	}

	bIsActiveSpawnClone = true;
	
	Blackboard->SetValueAsBool(TEXT("bActiveSpawnClone"), true);
}

FGameplayTag ABoss4AIController::GetAttack()
{
	APawn* Boss4Pawn = GetPawn();

	if (!IsValid(Boss4Pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4AIController::GetAttack()] Failed: No Owner"));
		return SkillTags::Boss4BackDash;
	}

	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4AIController::GetAttack()] Failed: No Blackboard"));
		return SkillTags::Boss4BackDash;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));

	if (!IsValid(TargetObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4AIController::GetAttack()] Failed: No TargetActor"));
		return SkillTags::Boss4BackDash;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4AIController::GetAttack()] Failed: TargetActor Not Actor"));
		return SkillTags::Boss4BackDash;
	}

	FVector OwnerLocation = Boss4Pawn->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	// Z값 차이를 고려 안 하기 위해 2D
	float Distance = FVector::Dist2D(OwnerLocation, TargetLocation);

	bool bCanBaseAttack = Distance < 400.f;
	bool bCanFlameStrike = Distance > 1000.f;
	bool bCanInfernoSpike = Distance > 500.f && Distance < 3000.f;
	bool bCanHellfireFall = Distance < 3000.f && bIsActiveHellfireFall && !bIsClone;
	bool bCanBlazeSphere = Distance > 1000.f && Distance < 3000.f && bIsActiveBlazeSphere && !bIsClone;
	bool bCanWeaponAttack1 = Distance < 1000.f && bIsActiveWeaponAttack1 && !bIsClone;
	bool bCanBackDash = Distance < 500.f;
	bool bCanTeleport = Distance < 500.f && bIsActiveTeleport;

	int32 TotalWeight = 0;

	// 추후에 배열 등으로 정규화 필요?

	if (bCanBaseAttack)
	{
		TotalWeight += CurrentBaseAttackWeight;
	}

	if (bCanFlameStrike)
	{
		TotalWeight += CurrentFlameStrikeWeight;
	}

	if (bCanInfernoSpike)
	{
		TotalWeight += CurrentInfernoSpikeWeight;
	}

	if (bCanHellfireFall)
	{
		TotalWeight += CurrentHellfireFallWeight;
	}

	if (bCanBlazeSphere)
	{
		TotalWeight += CurrentBlazeSphereWeight;
	}

	if (bCanWeaponAttack1)
	{
		TotalWeight += CurrentWeaponAttack1Weight;
	}

	if (bCanBackDash)
	{
		TotalWeight += CurrentBackDashWeight;
	}

	if (bCanTeleport)
	{
		TotalWeight += CurrentTeleportWeight;
	}

	int32 RandomWeight = FMath::RandRange(1, TotalWeight);

	FGameplayTag ReturnTag = SkillGroupTags::Boss4BackDash;

	if (bCanBaseAttack)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentBaseAttackWeight)
		{
			ReturnTag = SkillGroupTags::BaseAttack;
			CurrentBaseAttackWeight = DefaultBaseAttackWeight;
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentBaseAttackWeight;
			CurrentBaseAttackWeight += BaseAttackWeightIncrement;
		}
	}

	if (bCanFlameStrike)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentFlameStrikeWeight)
		{
			ReturnTag = SkillGroupTags::Boss4FlameStrike;
			CurrentFlameStrikeWeight = DefaultFlameStrikeWeight;
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentFlameStrikeWeight;
			CurrentFlameStrikeWeight += FlameStrikeWeightIncrement;
		}
	}

	if (bCanInfernoSpike)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentInfernoSpikeWeight)
		{
			ReturnTag = SkillGroupTags::Boss4InfernoSpike;
			CurrentInfernoSpikeWeight = DefaultInfernoSpikeWeight;
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentInfernoSpikeWeight;
			CurrentInfernoSpikeWeight += InfernoSpikeWeightIncrement;
		}
	}

	if (bCanHellfireFall)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentHellfireFallWeight)
		{
			ReturnTag = SkillGroupTags::Boss4HellfireFall;
			CurrentHellfireFallWeight = DefaultHellfireFallWeight;
			DeactivateSkill(&bIsActiveHellfireFall, HellfireFallCoolDown, &HellfireFallCoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentHellfireFallWeight;
			CurrentHellfireFallWeight += HellfireFallWeightIncrement;
		}
	}

	if (bCanBlazeSphere)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentBlazeSphereWeight)
		{
			ReturnTag = SkillGroupTags::Boss4BlazeSphere;
			CurrentBlazeSphereWeight = DefaultBlazeSphereWeight;
			DeactivateSkill(&bIsActiveBlazeSphere, BlazeSphereCoolDown, &BlazeSphereCoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentBlazeSphereWeight;
			CurrentBlazeSphereWeight += BlazeSphereWeightIncrement;
		}
	}

	if (bCanWeaponAttack1)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentWeaponAttack1Weight)
		{
			ReturnTag = SkillGroupTags::Boss4WeaponAttack1Start;
			CurrentWeaponAttack1Weight = DefaultWeaponAttack1Weight;
			DeactivateSkill(&bIsActiveWeaponAttack1, WeaponAttack1CoolDown, &WeaponAttack1CoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentWeaponAttack1Weight;
			CurrentWeaponAttack1Weight += WeaponAttack1WeightIncrement;
		}
	}

	if (bCanBackDash)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentBackDashWeight)
		{
			ReturnTag = SkillGroupTags::Boss4BackDash;
			CurrentBackDashWeight = DefaultBackDashWeight;
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentBackDashWeight;
			CurrentBackDashWeight += BackDashWeightIncrement;
		}
	}

	if (bCanTeleport)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentTeleportWeight)
		{
			ReturnTag = SkillGroupTags::Boss4Teleport;
			CurrentTeleportWeight = DefaultTeleportWeight;
			DeactivateSkill(&bIsActiveTeleport, TeleportCoolDown, &TeleportCoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentTeleportWeight;
			CurrentTeleportWeight += TeleportWeightIncrement;
		}
	}

    return ReturnTag;
}

void ABoss4AIController::DeactivateSkill(bool* IsActive, float CoolDown, FTimerHandle* CoolDownTimerHandle)
{
	*IsActive = false;
	TWeakObjectPtr<ABoss4AIController> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(
		*CoolDownTimerHandle,
		[WeakThis, IsActive]()
		{
			if (!WeakThis.IsValid())
			{
				return;
			}

			*IsActive = true;
		},
		CoolDown,
		false
	);
}