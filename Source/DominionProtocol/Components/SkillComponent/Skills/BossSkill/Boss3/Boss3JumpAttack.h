// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss3JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss3JumpAttack : public UBaseSkill
{
	GENERATED_BODY()
public:
	UBoss3JumpAttack();

protected:
	virtual void Execute() override;

	void LaunchParabolicAtTarget(AActor* TargetActorA, float TimeToReach);

private:
	UPROPERTY()
	AActor* TargetActor1;

	void JumpToTarget();
};
