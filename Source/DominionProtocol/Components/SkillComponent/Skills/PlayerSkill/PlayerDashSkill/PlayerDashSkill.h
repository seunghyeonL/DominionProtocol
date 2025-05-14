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

	FVector DashMoveDirection;

	virtual void Execute() override;
	virtual void Tick(float DeltaTime) override;

	/** Dash 방향 계산 (입력 없으면 뒤로) */
	void SetDashDirection();

	FTimerHandle DurationTimer;
	
	// === Dash 수치 설정값 ===
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashSpeed;
	
	
};


