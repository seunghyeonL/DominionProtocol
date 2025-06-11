// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/BTDecorator_CheckTargetDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTDecorator_CheckTargetDistance::UBTDecorator_CheckTargetDistance()
{
	NodeName = "Check Target Distance (Min ~ Max)";
}

bool UBTDecorator_CheckTargetDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;

	if (!AIPawn || !TargetActor) return false;

	const float Distance = FVector::Dist(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());

	return Distance >= MinDistance && Distance <= MaxDistance;
}