// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RunNodeIfHasAllTag.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API URunNodeIfHasAllTag : public UBTDecorator
{
	GENERATED_BODY()
public:
	URunNodeIfHasAllTag();

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTagContainer RunningTags;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
