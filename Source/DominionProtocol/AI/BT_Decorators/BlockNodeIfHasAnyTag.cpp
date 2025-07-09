﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockNodeIfHasAnyTag.h"
#include "AIController.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"

UBlockNodeIfHasAnyTag::UBlockNodeIfHasAnyTag()
{
	NodeName = TEXT("Block Node If Has Any Tag");
	bNotifyCeaseRelevant = true; // for use AbortMode Self
	bNotifyTick = true; // for use tick
}

bool UBlockNodeIfHasAnyTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		Debug::PrintError(TEXT("UBlockNodeIfHasAnyTag::CalculateRawConditionValue : Invalid AIController."));
		return false;
	}

	AActor* OwnerActor = AIController->GetPawn();
	if (!OwnerActor)
	{
		Debug::PrintError(TEXT("UBlockNodeIfHasAnyTag::CalculateRawConditionValue : Invalid OwnerActor."));
		return false;
	}

	if (auto StatusComponentUser = Cast<IStatusComponentUser>(OwnerActor))
	{
		FGameplayTagContainer& ActiveTags = StatusComponentUser->GetActiveStatusEffectTags();
		
		return !ActiveTags.HasAny(BlockingTags);
	}

	Debug::PrintError(TEXT("UBlockNodeIfHasAllTag::CalculateRawConditionValue : OwnerActor doesn't implement StatusComponentUser."));
	return true;
}

void UBlockNodeIfHasAnyTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const bool bShouldContinue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bShouldContinue)
	{
		// 조건이 더 이상 충족되지 않으면 스스로 Abort
		OwnerComp.RequestExecution(this);
	}
}
