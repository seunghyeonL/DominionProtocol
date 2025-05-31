// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllers/BaseAIController.h"
#include "Boss1AIController.generated.h"

/**
 * 
 */

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
protected:
	virtual void BeginPlay() override;

private:
	// 현재 가중치
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentBaseAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentSuperAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentSpecialAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentEvadeAttackWeight;

	// 초기 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBaseAttackWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultSuperAttackWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultSpecialAttackWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultEvadeAttackWeight;

	// 가중치 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BaseAttackWeightIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 SuperAttackWeightIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 SpecialAttackWeightIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 EvadeAttackWeightIncrement;

	// 쿨 있는 스킬들 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	float SpecialAttackCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	float RangedAttackCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	float EvadeAttackCoolDown;

	// 쿨 있는 스킬들 활성화 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveSpecialAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveRangedAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveEvadeAttack;

	void UpdateWeights();

	FTimerHandle SpecialAttackCoolDownTimer;
	FTimerHandle RangedAttackCoolDownTimer;
	FTimerHandle EvadeAttackCoolDownTimer;
};
