// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/Boss2AIController.h"

#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Util/DebugHelper.h"

ABoss2AIController::ABoss2AIController()
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

FGameplayTag ABoss2AIController::GetAttack()
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

	RandomWeight -= SuperAttackWeight;

	if (bIsActiveSpecialAttack && RandomWeight <= SpecialAttackWeight)
	{
		SpecialAttackWeight = -1;
		UpdateWeights();
		DeactivateSpecialAttack();

		return SkillGroupTags::SpecialAttack;
	}

	EvadeAttackWeight = -1;
	UpdateWeights();
	DeactivateEvadeAttack();

	return SkillTags::Boss1EvadeAttack;
}

bool ABoss2AIController::CanAttack(FGameplayTag SkillTag)
{
	if (SkillTag == SkillTags::Boss1RangedAttack && bIsActiveRangedAttack)
	{
		DeactivateRangedAttack();

		return true;
	}

	return false;
}

void ABoss2AIController::DeactivateSpecialAttack()
{
	bIsActiveSpecialAttack = false;

	TWeakObjectPtr<ABoss2AIController> WeakThis(this);

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

void ABoss2AIController::ActivateSpecialAttack()
{
	bIsActiveSpecialAttack = true;
}

void ABoss2AIController::DeactivateRangedAttack()
{
	bIsActiveRangedAttack = false;

	TWeakObjectPtr<ABoss2AIController> WeakThis(this);

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

void ABoss2AIController::ActivateRangedAttack()
{
	bIsActiveRangedAttack = true;
}

void ABoss2AIController::DeactivateEvadeAttack()
{
	bIsActiveEvadeAttack = false;

	TWeakObjectPtr<ABoss2AIController> WeakThis(this);

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

void ABoss2AIController::ActivateEvadeAttack()
{
	bIsActiveEvadeAttack = true;
}

void ABoss2AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TargetActor)
	{
		TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
		return;
	}
	if (APawn* AIPawn = GetPawn())
	{
		if (auto StatusComponentUser = Cast<IStatusComponentUser>(AIPawn))
		{
			if (StatusComponentUser->GetActiveStatusEffectTags().HasTagExact(EffectTags::Death))
			{
				return;
			}
		}
		const FVector Direction = (TargetActor->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal();
		FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		LookRotation.Pitch = 0.f;
		LookRotation.Roll = 0.f;
		AIPawn->SetActorRotation(LookRotation);
	}
}

void ABoss2AIController::UpdateWeights()
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
