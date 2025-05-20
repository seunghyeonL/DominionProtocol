// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/CheckAIStateEquals.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/StatusComponent/StatusComponentUser.h"

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

	IStatusComponentUser* StatusUser = Cast<IStatusComponentUser>(ControlledPawn);
	if (!StatusUser) return false;

	const FGameplayTagContainer& ActiveTags = StatusUser->GetActiveStatusEffectTags();
	UE_LOG(LogTemp, Warning, TEXT("[CheckAIStateEquals] Required: %s / Active: %s"),
		*RequiredStateTag.ToString(),
		*ActiveTags.ToString());
	return ActiveTags.HasTagExact(RequiredStateTag);
}