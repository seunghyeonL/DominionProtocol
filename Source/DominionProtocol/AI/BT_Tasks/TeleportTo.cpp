// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTeleportTo::UTeleportTo()
{
	NodeName = "TeleportTo";
}

EBTNodeResult::Type UTeleportTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	FVector TeleportToLocation = Blackboard->GetValueAsVector(LocationKey.SelectedKeyName);
	
	bool bSuccess = AIPawn->TeleportTo(TeleportToLocation, AIPawn->GetActorRotation());
	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
