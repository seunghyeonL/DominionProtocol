// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockNodeIfHasAnyTag.h"
#include "AIController.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"

UBlockNodeIfHasAnyTag::UBlockNodeIfHasAnyTag()
{
	NodeName = TEXT("Block Node If Has Any Tag");
}

bool UBlockNodeIfHasAnyTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* OwnerActor = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!OwnerActor)
	{
		Debug::PrintError(TEXT("UBlockNodeIfHasAnyTag::CalculateRawConditionValue : Invalid OwnerActor."));
		return false;
	}

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerActor))
	{
		FGameplayTagContainer ActiveTags = StatusComponentUser->GetActiveStatusEffectTags();
		
		return !ActiveTags.HasAny(BlockingTags);
	}

	Debug::PrintError(TEXT("UBlockNodeIfHasAllTag::CalculateRawConditionValue : OwnerActor doesn't implement StatusComponentUser."));
	return true;
}
