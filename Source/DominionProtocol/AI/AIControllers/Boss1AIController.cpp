// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/Boss1AIController.h"

ABoss1AIController::ABoss1AIController()
{
	BaseAttackWeight = 3;
	SuperAttackWeight = 2;
	SpecialAttackWeight = 1;
	EvadeAttackWeight = 0;

	SpecialAttackCoolDown = 20.f;
	RangedAttackCoolDown = 20.f;
	EvadeAttackCoolDown = 20.f;

	bIsActiveSpecialAttack = true;
	bIsActiveRangedAttack = true;
	bIsActiveEvadeAttack = true;
}

FGameplayTag ABoss1AIController::GetAttack()
{
	int32 TotalWeight = BaseAttackWeight + SuperAttackWeight;

	if (bIsActiveSpecialAttack)
	{
		TotalWeight += SpecialAttackWeight;
	}

	if (bIsActiveEvadeAttack)
	{
		TotalWeight += EvadeAttackWeight;
	}

	int32 RandomWeight = FMath::RandRange(1, TotalWeight);

	if (RandomWeight <= BaseAttackWeight)
	{
		BaseAttackWeight = 0;
		UpdateWeights();

		return SkillGroupTags::BaseAttack;
	}

	RandomWeight -= BaseAttackWeight;

	if (RandomWeight <= SuperAttackWeight)
	{
		SuperAttackWeight = -1;
		UpdateWeights();

		return SkillGroupTags::SuperAttack;
	}

	if (bIsActiveSpecialAttack && RandomWeight <= SpecialAttackCoolDown)
	{
		SpecialAttackWeight = -1;
		UpdateWeights();
		DeactivateSpecialAttack();

		return SkillGroupTags::SpecialAttack;
	}

	EvadeAttackWeight = -1;
	UpdateWeights();
	DeactivateSpecialAttack();

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
	BaseAttackWeight += 3;
	SuperAttackWeight += 3;

	if (bIsActiveSpecialAttack)
	{
		SpecialAttackWeight += 2;
	}

	if (bIsActiveEvadeAttack)
	{
		EvadeAttackWeight += 2;
	}
}
