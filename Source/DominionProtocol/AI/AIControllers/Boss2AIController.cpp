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

	BaseAttack1WeightIncrement = 3;
	BaseAttack2WeightIncrement = 3;
}

void ABoss2AIController::BeginPlay()
{
	Super::BeginPlay();

	CurrentBaseAttack1Weight = DefaultBaseAttack1Weight;
	CurrentBaseAttack2Weight = DefaultBaseAttack2Weight;
}

FGameplayTag ABoss2AIController::GetAttack()
{
	int32 TotalWeight = CurrentBaseAttack1Weight + CurrentBaseAttack2Weight;

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
	return SkillGroupTags::Boss2BaseAttack2;
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
	CurrentBaseAttack1Weight += BaseAttack1WeightIncrement;
	CurrentBaseAttack2Weight += BaseAttack2WeightIncrement;
}
