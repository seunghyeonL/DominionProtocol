// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTDecorator_CheckTargetDistance : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckTargetDistance();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (ClampMin = "0.0"))
	float MinDistance = 0.f;

	UPROPERTY(EditAnywhere, Category = "Condition", meta = (ClampMin = "0.0"))
	float MaxDistance = 1000.f;
};
