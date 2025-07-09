// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecutePattern.h"
#include "DashTo.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UDashTo : public UExecutePattern
{
	GENERATED_BODY()
public:
	UDashTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnAnimationCompleted() override;
};
