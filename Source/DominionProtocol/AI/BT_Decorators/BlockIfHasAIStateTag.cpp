// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/BlockIfHasAIStateTag.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/StatusComponent/StatusComponentUser.h"

UBlockIfHasAIStateTag::UBlockIfHasAIStateTag()
{
	NodeName = TEXT("Block If Has AI State Tag");
}

bool UBlockIfHasAIStateTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return true;

	const APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return true;

	AActor* OwnerActor = AIController->GetPawn();
	if (!OwnerActor)return false;

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerActor))
	{
		FGameplayTagContainer& ActiveTags = StatusComponentUser->GetActiveStatusEffectTags();

		return !ActiveTags.HasAll(BlockingAIStateTags);
	}
	return true;
}

void UBlockIfHasAIStateTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const bool bShouldContinue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bShouldContinue)
	{
		OwnerComp.RequestExecution(this);
	}
}