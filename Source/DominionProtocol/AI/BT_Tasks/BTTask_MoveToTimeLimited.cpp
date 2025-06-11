// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Tasks/BTTask_MoveToTimeLimited.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_MoveToTimeLimited::UBTTask_MoveToTimeLimited()
{
	bNotifyTick = true;
	NodeName = "MoveTo with Time Limit";
}

EBTNodeResult::Type UBTTask_MoveToTimeLimited::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ElapsedTime = 0.f;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_MoveToTimeLimited::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= MaxMoveTime)
	{
		if (AAIController* AICon = OwnerComp.GetAIOwner())
		{
			//AICon->StopMovement();
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
