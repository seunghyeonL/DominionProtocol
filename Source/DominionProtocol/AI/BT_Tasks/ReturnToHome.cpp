// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Tasks/ReturnToHome.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIState_Idle.h"

UReturnToHome::UReturnToHome()
{
	NodeName = TEXT("Return To Home");
	bNotifyTick = true;

	HomeLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UReturnToHome, HomeLocationKey));
}

EBTNodeResult::Type UReturnToHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	ControlledActor = AICon->GetPawn();
	if (!ControlledActor) return EBTNodeResult::Failed;

	FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(HomeLocationKey.SelectedKeyName);
	TargetLocation = HomeLocation;

	AICon->MoveToLocation(TargetLocation, AcceptableRadius);
	bMoving = true;

	return EBTNodeResult::InProgress;
}

void UReturnToHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!ControlledActor) return;

	const float Distance = FVector::Dist(ControlledActor->GetActorLocation(), TargetLocation);
	UE_LOG(LogTemp, Warning, TEXT("Distance to Home: %f"), Distance);
	if (Distance <= AcceptableRadius)
	{
		// 도착 → Idle 상태로 전이
		if (UStatusComponent* StatusComp = ControlledActor->FindComponentByClass<UStatusComponent>())
		{
			UAIState_Idle* IdleState = NewObject<UAIState_Idle>(StatusComp);
			IdleState->Activate();
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

