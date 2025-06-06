// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChooseRandomPattern.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTService_ChooseRandomPattern : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_ChooseRandomPattern();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


public:
	// 최대 패턴 인덱스 (0 ~ MaxPatternIndex 범위)
	UPROPERTY(EditAnywhere, Category = "Random Pattern")
	int32 MaxPatternIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector PatternKey;
};
