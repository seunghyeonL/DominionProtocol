// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/CheckAIStateEquals.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/StatusComponent/AIStatusComponent.h"

UCheckAIStateEquals::UCheckAIStateEquals()
{
	NodeName = TEXT("Check AI State == Tag");
}

bool UCheckAIStateEquals::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return false;

	UAIStatusComponent* AIStatus = ControlledPawn->FindComponentByClass<UAIStatusComponent>();
	if (!AIStatus) return false;

	const FGameplayTag& CurrentTag = AIStatus->GetCurrentStateTag();

	const bool bMatch = (CurrentTag == RequiredStateTag);

	return bMatch;
}