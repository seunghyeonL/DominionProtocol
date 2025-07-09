// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss3Grab.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss3Grab : public UBaseSkill
{
	GENERATED_BODY()
public:
	UBoss3Grab();

	virtual void AttackTrace() const override;

	void SpawnHandMesh(const FHitResult& Hit) const;
};
