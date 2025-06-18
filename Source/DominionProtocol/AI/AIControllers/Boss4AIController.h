// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllers/BaseAIController.h"
#include "Boss4AIController.generated.h"

/**
 * 
 */

class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API ABoss4AIController : public ABaseAIController
{
	GENERATED_BODY()
	
public:
	ABoss4AIController();

	virtual void Tick(float DeltaTime) override;

	virtual FGameplayTag GetAttack() override;

protected:
	virtual void BeginPlay() override;
	

private:
	void DeactivateSkill(bool* IsActive, float CoolDown, FTimerHandle* CoolDownTimerHandle);

	// 현재 가중치
	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentBaseAttackWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentFlameStrikeWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentInfernoSpikeWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentHellfireFallWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentBlazeSphereWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentWeaponAttack1Weight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentBackDashWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentTeleportWeight;

	// 초기 가중치
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBaseAttackWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultFlameStrikeWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultInfernoSpikeWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultHellfireFallWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBlazeSphereWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultWeaponAttack1Weight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultBackDashWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultTeleportWeight;

	// 가중치 증가량
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BaseAttackWeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 FlameStrikeWeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 InfernoSpikeWeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 HellfireFallWeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BlazeSphereWeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 WeaponAttack1WeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 BackDashWeightIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 TeleportWeightIncrement;

	// 쿨 있는 스킬들 쿨타임
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float HellfireFallCoolDown;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float BlazeSphereCoolDown;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float WeaponAttack1CoolDown;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float TeleportCoolDown;

	// 쿨 있는 스킬들 활성화 여부
	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveHellfireFall;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveBlazeSphere;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveWeaponAttack1;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveTeleport;

	FTimerHandle HellfireFallCoolDownTimer;
	FTimerHandle BlazeSphereCoolDownTimer;
	FTimerHandle WeaponAttack1CoolDownTimer;
	FTimerHandle TeleportCoolDownTimer;

	// 클론이 사용할 수 없는 스킬을 지정할 때 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clone", meta = (AllowPrivateAccess = true))
	bool bIsClone;

	// 스폰 클론 스킬을 사용했는지 판별할 때 사용
	bool bIsActiveSpawnClone;

	UStatusComponent* CachedStatusComponent;

	float CachedMaxHealth;
};
