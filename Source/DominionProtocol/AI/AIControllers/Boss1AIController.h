// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllers/BaseAIController.h"
#include "Boss1AIController.generated.h"

/**
 * 
 */

class UBTTaskNode;

UCLASS()
class DOMINIONPROTOCOL_API ABoss1AIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	ABoss1AIController();

	FGameplayTag GetAttack();

	bool CanAttack(FGameplayTag SkillTag);

	void DeactivateSpecialAttack();
	void ActivateSpecialAttack();

	void DeactivateRangedAttack();
	void ActivateRangedAttack();

	void DeactivateEvadeAttack();
	void ActivateEvadeAttack();

	void SetCachedTask(UBTTaskNode* NewCachedTask);
	void ClearCachedTask();
	
private:
	int32 BaseAttackWeight;
	int32 SuperAttackWeight;
	int32 SpecialAttackWeight;
	int32 EvadeAttackWeight;

	float SpecialAttackCoolDown;
	float RangedAttackCoolDown;
	float EvadeAttackCoolDown;

	bool bIsActiveSpecialAttack;
	bool bIsActiveRangedAttack;
	bool bIsActiveEvadeAttack;

	void UpdateWeights();

	FTimerHandle SpecialAttackCoolDownTimer;
	FTimerHandle RangedAttackCoolDownTimer;
	FTimerHandle EvadeAttackCoolDownTimer;

	UBTTaskNode* CachedTask;
};
