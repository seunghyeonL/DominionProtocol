// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanActivateSkill.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTDecorator_CanActivateSkill : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanActivateSkill();

protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	FGameplayTag CheckSkillTag;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
