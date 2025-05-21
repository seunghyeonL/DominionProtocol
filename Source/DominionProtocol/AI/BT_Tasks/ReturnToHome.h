// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "ReturnToHome.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UReturnToHome : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UReturnToHome();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector HomeLocationKey;

	UPROPERTY()
	AActor* ControlledActor;

	FVector TargetLocation;
	float AcceptableRadius = 100;
	bool bMoving = false;
};