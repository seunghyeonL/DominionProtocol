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
	void DeactivateSkill(bool* IsActive, float CoolDown , FTimerHandle* CoolDownTimerHandle);

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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentDashAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentTeleportAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentEvadeAttackWeight;

	// 초기 가중치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBaseAttack1Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBaseAttack2Weight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultDashAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultTeleportAttackWeight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultEvadeAttackWeight;

	// 가중치 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BaseAttack1WeightIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BaseAttack2WeightIncrement;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DashAttackWeightIncrement;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 TeleportAttackWeightIncrement;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 EvadeAttackWeightIncrement;

	// 쿨 있는 스킬들 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	float TeleportAttackCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	float EvadeAttackCoolDown;
	
	// 쿨 있는 스킬들 활성화 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveTeleportAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveEvadeAttack;
	
	void UpdateWeights();
	
	FTimerHandle TeleportAttackCoolDownTimer;
	FTimerHandle EvadeAttackCoolDownTimer;
};
