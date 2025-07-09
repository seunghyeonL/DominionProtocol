// Fill out your copyright notice in the Description page of Project Settings.


#include "DashTo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Util/DebugHelper.h"

UDashTo::UDashTo()
{
	NodeName = "DashTo";
}

EBTNodeResult::Type UDashTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		Debug::PrintError("UDashNode::ExecuteTask: Blackboard is null");
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsBool(TEXT("bIsDash"), true);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UDashTo::OnAnimationCompleted()
{
	UBlackboardComponent* Blackboard = CachedTemp->GetBlackboardComponent();
	if (!Blackboard)
	{
		Debug::PrintError("UDashNode::ExecuteTask: Blackboard is null");
		return;
	}
	Blackboard->SetValueAsBool(TEXT("bIsDash"), false);
	Super::OnAnimationCompleted();
}
