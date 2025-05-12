// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WaitUntilTagsRemoved.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UWaitUntilTagsRemoved : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UWaitUntilTagsRemoved();

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer WaitingTags;

	UPROPERTY(EditAnywhere)
	float TickInterval;
	
	float Time;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
