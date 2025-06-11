// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToTimeLimited.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTTask_MoveToTimeLimited : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToTimeLimited();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MaxMoveTime = 1.5f;

	float ElapsedTime = 0.f;
};
