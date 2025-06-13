// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/Boss2AIController.h"

#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Util/DebugHelper.h"

#include "Util/DebugHelper.h"

ABoss2AIController::ABoss2AIController()
{
	DefaultBaseAttack1Weight = 3;
	DefaultBaseAttack2Weight = 3;
	DefaultDashAttackWeight = 3;
	DefaultTeleportAttackWeight = 0;
	DefaultEvadeAttackWeight = 0;

	BaseAttack1WeightIncrement = 3;
	BaseAttack2WeightIncrement = 3;
	DashAttackWeightIncrement = 3;
	TeleportAttackWeightIncrement = 1;
	EvadeAttackWeightIncrement = 4;

	EvadeAttackCoolDown = 10.f;
	TeleportAttackCoolDown = 15.f;
	
	bIsActiveTeleportAttack = true;
	bIsActiveEvadeAttack = true;
}

void ABoss2AIController::BeginPlay()
{
	Super::BeginPlay();

	CurrentBaseAttack1Weight = DefaultBaseAttack1Weight;
	CurrentBaseAttack2Weight = DefaultBaseAttack2Weight;
}

FGameplayTag ABoss2AIController::GetAttack()
{
	int32 TotalWeight = CurrentBaseAttack1Weight + CurrentBaseAttack2Weight + CurrentDashAttackWeight;
	if (bIsActiveTeleportAttack)
	{
		TotalWeight += CurrentTeleportAttackWeight;
	}

	if (bIsActiveEvadeAttack)
	{
		TotalWeight += CurrentEvadeAttackWeight;
	}

	int32 RandomWeight = FMath::RandRange(1, TotalWeight);

	if (RandomWeight <= CurrentBaseAttack1Weight)
	{
		UpdateWeights();
		CurrentBaseAttack1Weight = DefaultBaseAttack1Weight;

		return SkillGroupTags::Boss2BaseAttack1;
	}

	RandomWeight -= CurrentBaseAttack1Weight;

	if (RandomWeight <= CurrentBaseAttack2Weight)
	{
		UpdateWeights();
		CurrentBaseAttack2Weight = DefaultBaseAttack2Weight;

		return SkillGroupTags::Boss2BaseAttack2;
	}

	RandomWeight -= CurrentBaseAttack2Weight;

	if (RandomWeight <= CurrentDashAttackWeight)
	{
		UpdateWeights();
		CurrentDashAttackWeight = DefaultDashAttackWeight;

		return SkillGroupTags::Boss2DashAttack;
	}

	RandomWeight -= CurrentDashAttackWeight;

	if (RandomWeight <= CurrentTeleportAttackWeight)
	{
		UpdateWeights();
		CurrentTeleportAttackWeight = DefaultTeleportAttackWeight;
		DeactivateSkill(&bIsActiveTeleportAttack, TeleportAttackCoolDown, &TeleportAttackCoolDownTimer);

		return SkillGroupTags::Boss2TeleportAttack;
	}

	CurrentEvadeAttackWeight = DefaultEvadeAttackWeight;
	DeactivateSkill(&bIsActiveEvadeAttack, EvadeAttackCoolDown, &EvadeAttackCoolDownTimer);
	
	return SkillGroupTags::Boss2EvadeAttack;
}

void ABoss2AIController::DeactivateSkill(bool* IsActive, float CoolDown, FTimerHandle* CoolDownTimerHandle)
{
	*IsActive = false;
	TWeakObjectPtr<ABoss2AIController> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(
		*CoolDownTimerHandle,
		[IsActive]()
		{
			*IsActive = true;
		},
		CoolDown,
		false
	);
}

void ABoss2AIController::UpdateWeights()
{
	CurrentBaseAttack1Weight += BaseAttack1WeightIncrement;
	CurrentBaseAttack2Weight += BaseAttack2WeightIncrement;
	CurrentDashAttackWeight += DashAttackWeightIncrement;
	CurrentTeleportAttackWeight += TeleportAttackWeightIncrement;
	CurrentEvadeAttackWeight += EvadeAttackWeightIncrement;
}
