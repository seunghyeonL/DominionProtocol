// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/Boss1AIController.h"

ABoss1AIController::ABoss1AIController()
{
	DefaultBaseAttackWeight = 3;
	DefaultSuperAttackWeight = 2;
	DefaultSpecialAttackWeight = 1;
	DefaultEvadeAttackWeight = 0;

	BaseAttackWeightIncrement = 3;
	SuperAttackWeightIncrement = 3;
	SpecialAttackWeightIncrement = 2;
	EvadeAttackWeightIncrement = 2;

	SpecialAttackCoolDown = 20.f;
	RangedAttackCoolDown = 20.f;
	EvadeAttackCoolDown = 20.f;

	bIsActiveSpecialAttack = true;
	bIsActiveRangedAttack = true;
	bIsActiveEvadeAttack = true;
}

void ABoss1AIController::BeginPlay()
{
	Super::BeginPlay();

	CurrentBaseAttackWeight = DefaultBaseAttackWeight;
	CurrentSuperAttackWeight = DefaultSuperAttackWeight;
	CurrentSpecialAttackWeight = DefaultSpecialAttackWeight;
	CurrentEvadeAttackWeight = DefaultEvadeAttackWeight;
}

FGameplayTag ABoss1AIController::GetAttack()
{
	int32 TotalWeight = CurrentBaseAttackWeight + CurrentSuperAttackWeight;

	if (bIsActiveSpecialAttack)
	{
		TotalWeight += CurrentSpecialAttackWeight;
	}

	if (bIsActiveEvadeAttack)
	{
		TotalWeight += CurrentEvadeAttackWeight;
	}

	int32 RandomWeight = FMath::RandRange(1, TotalWeight);

	if (RandomWeight <= CurrentBaseAttackWeight)
	{
		UpdateWeights();
		CurrentBaseAttackWeight = DefaultBaseAttackWeight;

		return SkillGroupTags::BaseAttack;
	}

	RandomWeight -= CurrentBaseAttackWeight;

	if (RandomWeight <= CurrentSuperAttackWeight)
	{
		UpdateWeights();
		CurrentSuperAttackWeight = DefaultSuperAttackWeight;

		return SkillGroupTags::SuperAttack;
	}

	RandomWeight -= CurrentSuperAttackWeight;

	if (bIsActiveSpecialAttack && RandomWeight <= CurrentSpecialAttackWeight)
	{
		UpdateWeights();
		CurrentSpecialAttackWeight = DefaultSpecialAttackWeight;
		DeactivateSpecialAttack();

		return SkillGroupTags::SpecialAttack;
	}

	CurrentEvadeAttackWeight = DefaultEvadeAttackWeight;
	DeactivateEvadeAttack();

	return SkillTags::Boss1EvadeAttack;
}

bool ABoss1AIController::CanAttack(FGameplayTag SkillTag)
{
	if (SkillTag == SkillTags::Boss1RangedAttack && bIsActiveRangedAttack)
	{
		DeactivateRangedAttack();

		return true;
	}

	return false;
}

void ABoss1AIController::DeactivateSpecialAttack()
{
	bIsActiveSpecialAttack = false;

	TWeakObjectPtr<ABoss1AIController> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(
		SpecialAttackCoolDownTimer,
		[WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->ActivateSpecialAttack();
			}
		},
		SpecialAttackCoolDown,
		false
	);
}

void ABoss1AIController::ActivateSpecialAttack()
{
	bIsActiveSpecialAttack = true;
}

void ABoss1AIController::DeactivateRangedAttack()
{
	bIsActiveRangedAttack = false;

	TWeakObjectPtr<ABoss1AIController> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(
		RangedAttackCoolDownTimer,
		[WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->ActivateRangedAttack();
			}
		},
		RangedAttackCoolDown,
		false
	);
}

void ABoss1AIController::ActivateRangedAttack()
{
	bIsActiveRangedAttack = true;
}

void ABoss1AIController::DeactivateEvadeAttack()
{
	bIsActiveEvadeAttack = false;

	TWeakObjectPtr<ABoss1AIController> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(
		EvadeAttackCoolDownTimer,
		[WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->ActivateEvadeAttack();
			}
		},
		EvadeAttackCoolDown,
		false
	);
}

void ABoss1AIController::ActivateEvadeAttack()
{
	bIsActiveEvadeAttack = true;
}

void ABoss1AIController::UpdateWeights()
{
	CurrentBaseAttackWeight += BaseAttackWeightIncrement;
	CurrentSuperAttackWeight += SuperAttackWeightIncrement;

	if (bIsActiveSpecialAttack)
	{
		CurrentSpecialAttackWeight += SpecialAttackWeightIncrement;
	}

	if (bIsActiveEvadeAttack)
	{
		CurrentEvadeAttackWeight += EvadeAttackWeightIncrement;
	}
}
