// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Tasks/BTTask_SwapPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SwapPatrolPoint::UBTTask_SwapPatrolPoint()
{
	NodeName = TEXT("Swap Patrol Point");
}

EBTNodeResult::Type UBTTask_SwapPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AActor* PatrolA = Cast<AActor>(BB->GetValueAsObject("PatrolPointA"));
	AActor* PatrolB = Cast<AActor>(BB->GetValueAsObject("PatrolPointB"));
	if (!PatrolA || !PatrolB) return EBTNodeResult::Failed;

	const FVector Current = BB->GetValueAsVector("CurrentPatrol");
	const FVector LocA = PatrolA->GetActorLocation();

	BB->SetValueAsVector("CurrentPatrol", Current.Equals(PatrolA->GetActorLocation(), 100.0f)
		? PatrolB->GetActorLocation()
		: PatrolA->GetActorLocation());


	return EBTNodeResult::Succeeded;
}
