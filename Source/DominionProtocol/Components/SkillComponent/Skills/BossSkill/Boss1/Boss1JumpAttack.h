// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss1JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss1JumpAttack : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UBoss1JumpAttack();

	virtual void Execute() override;

	void MoveToLocation();

private:
	FVector TargetLocation;
};
