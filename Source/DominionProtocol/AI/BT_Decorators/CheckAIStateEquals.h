// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CheckAIStateEquals.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UCheckAIStateEquals : public UBTDecorator
{
	GENERATED_BODY()
	public:
	UCheckAIStateEquals();

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTag RequiredStateTag;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
