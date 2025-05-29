// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectSkill.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTService_SelectSkill : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_SelectSkill();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
