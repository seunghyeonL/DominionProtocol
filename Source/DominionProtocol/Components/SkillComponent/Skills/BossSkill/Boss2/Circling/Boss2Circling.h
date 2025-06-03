// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss2Circling.generated.h"

/**
 * Player와 심리전을 하기 위해 랜덤한 시간으로 Player와 일정한 거리 사이에서 움직이는 스킬(Circling)
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss2Circling : public UBaseSkill
{
	GENERATED_BODY()
	// 지속시간 후 스킬 끝내는 TimerHandle
	FTimerHandle EndSkillTimerHandle;
	float CirclingDirection = 1;
	bool bIsCircling = false;
	void Circling() const;
	void EndSkill();

public:
	UBoss2Circling();
	
	virtual void Execute() override;
	virtual void Tick(float DeltaTime) override;
	// 심리전하는 최소 최대 시간
	float MinTime = 1.f;
	float MaxTime = 4.f;
};
