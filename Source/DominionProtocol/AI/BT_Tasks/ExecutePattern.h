// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ExecutePattern.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UExecutePattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UExecutePattern();

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag SkillGroupTag;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
