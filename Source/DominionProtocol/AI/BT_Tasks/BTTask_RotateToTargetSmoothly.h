// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToTargetSmoothly.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBTTask_RotateToTargetSmoothly : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_RotateToTargetSmoothly();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float RotationSpeed = 360.f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float AcceptableAngleDifference = 3.f;

private:
	APawn* ControlledPawn = nullptr;
	AActor* TargetActor = nullptr;
};
