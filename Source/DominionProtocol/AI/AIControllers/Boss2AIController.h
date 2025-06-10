// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllers/BaseAIController.h"
#include "Boss2AIController.generated.h"

/**
 * 
 */

UCLASS()
class DOMINIONPROTOCOL_API ABoss2AIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	ABoss2AIController();

	virtual FGameplayTag GetAttack() override;

protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	AActor* TargetActor;
private:
	// 현재 가중치
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentBaseAttack1Weight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentBaseAttack2Weight;

	// 초기 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBaseAttack1Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBaseAttack2Weight;

	// 가중치 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BaseAttack1WeightIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BaseAttack2WeightIncrement;

	void UpdateWeights();
};
