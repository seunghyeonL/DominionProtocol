// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Tasks/BTTask_RotateToTargetSmoothly.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToTargetSmoothly::UBTTask_RotateToTargetSmoothly()
{
	NodeName = TEXT("Rotate To Target Smoothly");
	bNotifyTick = true;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToTargetSmoothly, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_RotateToTargetSmoothly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToTargetSmoothly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!ControlledPawn || !TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FRotator CurrentRotation = ControlledPawn->GetActorRotation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());

	// Yaw만 회전
	float NewYaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, RotationSpeed * DeltaSeconds);
	FRotator NewRotation = FRotator(0.f, NewYaw, 0.f);
	ControlledPawn->SetActorRotation(NewRotation);

	// 얼마나 가까이 바라보고 있는가?
	float YawDiff = FMath::Abs(DesiredRotation.Yaw - NewYaw);
	if (YawDiff < AcceptableAngleDifference)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}