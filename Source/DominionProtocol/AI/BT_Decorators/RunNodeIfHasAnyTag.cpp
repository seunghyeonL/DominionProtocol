// Fill out your copyright notice in the Description page of Project Settings.


#include "RunNodeIfHasAnyTag.h"
#include "AIController.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"

URunNodeIfHasAnyTag::URunNodeIfHasAnyTag()
{
	NodeName = TEXT("Run Node If Has Any Tag");
}

bool URunNodeIfHasAnyTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* OwnerActor = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerActor)
	{
		Debug::PrintError(TEXT("URunNodeIfHasAnyTag::CalculateRawConditionValue : Invalid OwnerActor."));
		return false;
	}

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerActor))
	{
		FGameplayTagContainer ActiveTags = StatusComponentUser->GetActiveStatusEffectTags();
		
		return ActiveTags.HasAny(RunningTags);
	}

	Debug::PrintError(TEXT("UBlockNodeIfHasAllTag::CalculateRawConditionValue : OwnerActor doesn't implement StatusComponentUser."));
	return false;
}
