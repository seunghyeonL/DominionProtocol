// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/CheckAIStateEquals.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/AIComponent/AIStateComponent.h"


UCheckAIStateEquals::UCheckAIStateEquals()
{
	NodeName = TEXT("Check AI State == Tag");
}

/*bool UCheckAIStateEquals::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return false;

	UAIStateComponent* AIState = ControlledPawn->FindComponentByClass<UAIStateComponent>();
	if (!AIState) return false;

	const FGameplayTag& CurrentTag = AIState->GetCurrentStateTag();

	const bool bMatch = (CurrentTag == RequiredStateTag);

	return bMatch;
}*/

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

	UE_LOG(LogTemp, Warning, TEXT("[CheckAIStateEquals] Required: %s / Current: %s / Match: %s"),
		*RequiredStateTag.ToString(),
		*CurrentTag.ToString(),
		bMatch ? TEXT("TRUE") : TEXT("FALSE"));

	return bMatch;
}
