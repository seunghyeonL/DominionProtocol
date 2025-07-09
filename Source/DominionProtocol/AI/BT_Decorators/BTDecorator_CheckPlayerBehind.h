// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckPlayerBehind.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTDecorator_CheckPlayerBehind : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckPlayerBehind();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Condition")
	float RearAngleDegrees = 45.f;
};
