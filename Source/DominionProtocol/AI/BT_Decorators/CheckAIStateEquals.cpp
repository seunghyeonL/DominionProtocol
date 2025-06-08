// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/CheckAIStateEquals.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/AIComponent/AIStateComponent.h"


UCheckAIStateEquals::UCheckAIStateEquals()
{
	NodeName = TEXT("Check AI State == Tag");
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}


bool UCheckAIStateEquals::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckAIStateEquals] Failed: No AIController"));
		return false;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckAIStateEquals] Failed: No ControlledPawn"));
		return false;
	}

	UAIStateComponent* AIState = AIController->FindComponentByClass<UAIStateComponent>();
	if (!AIState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckAIStateEquals] Failed: No AIStateComponent"));
		return false;
	}

	const FGameplayTag& CurrentTag = AIState->GetCurrentStateTag();
	const bool bMatch = (CurrentTag == RequiredStateTag);

	return bMatch;
}

void UCheckAIStateEquals::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 상태가 일치하지 않으면 노드 중단 요청
	if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
	{
		OwnerComp.RequestExecution(this); // Observer Aborts: Self or Both 필요
	}
}