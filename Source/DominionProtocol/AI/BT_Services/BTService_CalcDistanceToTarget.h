// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CalcDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTService_CalcDistanceToTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CalcDistanceToTarget();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
