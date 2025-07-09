// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BlockIfHasAIStateTag.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBlockIfHasAIStateTag : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBlockIfHasAIStateTag();

protected:
	UPROPERTY(EditAnywhere, Category = "AIState")
	FGameplayTagContainer BlockingAIStateTags;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
