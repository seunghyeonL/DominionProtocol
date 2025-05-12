// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitUntilTagsRemoved.h"
#include "AIController.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"

UWaitUntilTagsRemoved::UWaitUntilTagsRemoved()
{
	Time = 0.f;
	TickInterval = 0.1f;
	bNotifyTick = true;
}

EBTNodeResult::Type UWaitUntilTagsRemoved::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UWaitUntilTagsRemoved::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Time < TickInterval)
	{
		Time += DeltaSeconds;
		return;
	}
	Time = 0.f;
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		Debug::PrintError(TEXT("UExecutePattern::ExecuteTask : Invalid AIController."));
		return;
	}

	AActor* OwnerActor = AIController->GetPawn();
	if (!OwnerActor)
	{
		Debug::PrintError(TEXT("UExecutePattern::ExecuteTask : Invalid OwnerActor."));
	}

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerActor))
	{
		FGameplayTagContainer ActiveEffects = StatusComponentUser->GetActiveStatusEffectTags();
		if (ActiveEffects.HasAny(WaitingTags))
		{
			return;
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		Debug::PrintError(TEXT("UWaitUntilTagsRemoved::TickTask : OwnerActor doesn't implement StatusComponentUser."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
