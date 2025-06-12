// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss2Dash.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss2Dash : public UBaseSkill
{
	GENERATED_BODY()

public:
	UBoss2Dash();
	FVector DashMoveDirection;

	virtual void Execute() override;
	virtual void Tick(float DeltaTime) override;

	void SetDashDirection();

	FTimerHandle DurationTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDuration;
};
