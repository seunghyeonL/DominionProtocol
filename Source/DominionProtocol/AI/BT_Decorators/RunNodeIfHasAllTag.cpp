// Fill out your copyright notice in the Description page of Project Settings.


#include "RunNodeIfHasAllTag.h"
#include "AIController.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"

URunNodeIfHasAllTag::URunNodeIfHasAllTag()
{
	NodeName = TEXT("Run Node If Has All Tag");
	bNotifyCeaseRelevant = true; // for use AbortMode Self
}

bool URunNodeIfHasAllTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		Debug::PrintError(TEXT("URunNodeIfHasAllTag::CalculateRawConditionValue : Invalid AIController."));
		return false;
	}

	AActor* OwnerActor = AIController->GetPawn();
	if (!OwnerActor)
	{
		Debug::PrintError(TEXT("URunNodeIfHasAllTag::CalculateRawConditionValue : Invalid OwnerActor."));
		return false;
	}

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerActor))
	{
		FGameplayTagContainer ActiveTags = StatusComponentUser->GetActiveStatusEffectTags();
		
		return ActiveTags.HasAll(RunningTags);
	}

	Debug::PrintError(TEXT("UBlockNodeIfHasAllTag::CalculateRawConditionValue : OwnerActor doesn't implement StatusComponentUser."));
	return false;
}
