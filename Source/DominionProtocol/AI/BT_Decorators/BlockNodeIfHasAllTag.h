// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BlockNodeIfHasAllTag.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBlockNodeIfHasAllTag : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBlockNodeIfHasAllTag();

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTagContainer BlockingTags;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
