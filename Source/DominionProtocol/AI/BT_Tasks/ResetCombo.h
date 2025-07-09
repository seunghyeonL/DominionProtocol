// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Util/GameTagList.h"
#include "ResetCombo.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UResetCombo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UResetCombo();
	UPROPERTY(EditAnywhere)
	FGameplayTag SkillGroupTag;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
