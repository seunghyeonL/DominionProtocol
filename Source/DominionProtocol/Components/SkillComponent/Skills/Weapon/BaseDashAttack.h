// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "BaseDashAttack.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBaseDashAttack : public UBaseSkill
{
	GENERATED_BODY()

public:
	UBaseDashAttack();

	virtual void Tick(float DeltaTime) override;

	virtual void Execute() override;

protected:
	float TimeElapsed;
	float MoveSpeed;
	float MoveDuration;
};
