// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/BTDecorator_CheckPlayerBehind.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTDecorator_CheckPlayerBehind::UBTDecorator_CheckPlayerBehind()
{
	NodeName = "Is Player Behind";
}

bool UBTDecorator_CheckPlayerBehind::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;

	if (!AIPawn || !TargetActor) return false;

	FVector ToTarget = (TargetActor->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal();
	FVector Forward = AIPawn->GetActorForwardVector();

	float Dot = FVector::DotProduct(Forward, ToTarget);
	float Degrees = FMath::Acos(Dot) * (180.f / PI);

	return Degrees >= (180.f - RearAngleDegrees);
}