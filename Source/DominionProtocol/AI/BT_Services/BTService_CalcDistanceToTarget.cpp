// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Services/BTService_CalcDistanceToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CalcDistanceToTarget::UBTService_CalcDistanceToTarget()
{
	NodeName = TEXT("Check Distance to Target");

	bNotifyTick = false;
	bNotifyBecomeRelevant = true;
}

void UBTService_CalcDistanceToTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTService_CalcDistanceToTarget] Failed: No AIController"));
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();

	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTService_CalcDistanceToTarget] Failed: No ControlledPawn"));
		return;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTService_CalcDistanceToTarget] Failed: No Blackboard"));
		return;
	}

	UObject* Target = Blackboard->GetValueAsObject("TargetActor");

	AActor* TargetActor = Cast<AActor>(Target);

	FVector OwnerLocation = ControlledPawn->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	// X, Y 좌표 거리만 계산
	OwnerLocation.Z = TargetLocation.Z;

	float Distance = FVector::Distance(OwnerLocation, TargetLocation);

	Blackboard->SetValueAsFloat("Distance", Distance);
}
