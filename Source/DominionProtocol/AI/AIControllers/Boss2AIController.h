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

	FGameplayTag GetAttack();

	bool CanAttack(FGameplayTag SkillTag);

	void DeactivateSpecialAttack();
	void ActivateSpecialAttack();

	void DeactivateRangedAttack();
	void ActivateRangedAttack();

	void DeactivateEvadeAttack();
	void ActivateEvadeAttack();
	
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
};
