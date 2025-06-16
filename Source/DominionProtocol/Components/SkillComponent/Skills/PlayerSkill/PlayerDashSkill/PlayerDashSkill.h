// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "PlayerDashSkill.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerDashSkill : public UBaseSkill
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlayerControlComponent> ControlComponent;

public:
	UPlayerDashSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;
	virtual void Execute() override;
	virtual void Tick(float DeltaTime) override;

	/** Dash 방향 계산 (입력 없으면 뒤로) */
	void SetDashDirection();

	FTimerHandle DurationTimer;

	FVector DashMoveDirection;
	
	// === Dash 수치 설정값 ===
	float DashSpeed;
	float DashDuration;
	
	float DashMoveStart;
	float DashMoveEnd;

	bool bIsMoving;

	float TimeElapsed;
};


