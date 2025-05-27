// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss1SpecialAttackFirst.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss1SpecialAttackFirst : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UBoss1SpecialAttackFirst();

	virtual void AttackTrace() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnHandMesh(const FHitResult& HitResult) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Object")
	TObjectPtr<UStaticMeshComponent> HitMesh = nullptr;
};
