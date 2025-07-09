// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecutePattern.h"
#include "DashNode.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UDashNode : public UExecutePattern
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnAnimationCompleted() override;
};
