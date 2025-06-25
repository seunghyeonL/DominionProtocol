// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIControllers/BaseAIController.h"
#include "Boss4Phase2AIController.generated.h"

/**
 * 
 */

class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API ABoss4Phase2AIController : public ABaseAIController
{
	GENERATED_BODY()
public:
	ABoss4Phase2AIController();

	virtual void Tick(float DeltaTime) override;

	virtual FGameplayTag GetAttack() override;

protected:
	virtual void BeginPlay() override;

private:
	void DeactivateSkill(bool* IsActive, float CoolDown, FTimerHandle* CoolDownTimerHandle);

	// 현재 가중치
	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentWeaponAttack2Weight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentWeaponAttack3Weight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentEvadeAttackWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentRisingWallWeight;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 CurrentSlashWaveWeight;

	// 초기 가중치
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultWeaponAttack2Weight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultWeaponAttack3Weight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultEvadeAttackWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultRisingWallWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 DefaultSlashWaveWeight;

	// 가중치 증가량
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 WeaponAttack2Increment;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 WeaponAttack3Increment;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 EvadeAttackIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 RisingWallIncrement;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	int32 SlashWaveIncrement;

	// 쿨 있는 스킬들 쿨타임
	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float WeaponAttack2CoolDown;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float EvadeAttackCoolDown;

	UPROPERTY(EditDefaultsOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	float RisingWallCoolDown;

	// 쿨 있는 스킬들 활성화 여부
	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveWeaponAttack2;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveEvadeAttack;

	UPROPERTY(VisibleInstanceOnly, Category = "Skill", meta = (AllowPrivateAccess = true))
	bool bIsActiveRisingWall;

	FTimerHandle WeaponAttack2CoolDownTimer;
	FTimerHandle EvadeAttackCoolDownTimer;
	FTimerHandle RisingWallCoolDownTimer;

	// 스폰 클론 스킬을 사용했는지 판별할 때 사용
	bool bIsActiveSpawnClone2Phase;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStatusComponent* CachedStatusComponent;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float CachedMaxHealth;
};
