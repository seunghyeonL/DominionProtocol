// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "PlayerDashSkill.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerDashSkill : public UBaseSkill
{
	GENERATED_BODY()

public:
	UPlayerDashSkill();

	virtual void Execute() override;
	virtual void Tick(float DeltaTime) override;

	/** Dash 시작: 스태미나 차감, 무적 시작, 이펙트 적용 */
	void StartDash();

	/** Dash 종료: 무적 해제 */
	void EndDash();

	/** 무적 상태 on/off */
	void SetInvincible(bool bInvincible);

	/** Dash 방향 계산 (입력 없으면 뒤로) */
	FVector GetDashDirection() const;

	FTimerHandle DurationTimer;
	
	// === Dash 수치 설정값 ===
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDistance = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashStaminaCost = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashMovedDistance = 0.f;

	UPROPERTY()
	FVector DashMoveDirection;
	
	// === Dash 이동용 변수 ===
	UPROPERTY()
	float DashElapsed = 0.f;
};


