// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "MagicLaserSkill.generated.h"

class UMnhCapsuleComponent;
class ALaserActor;

UCLASS()
class DOMINIONPROTOCOL_API UMagicLaserSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UMagicLaserSkill();

	virtual void Execute() override;

	virtual void StartTrace(const FGameplayTagContainer& TagContainer) override;
	virtual void StopTrace(const FGameplayTagContainer& TagContainer) override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const override;

	UPROPERTY()
	TObjectPtr<ALaserActor> LaserActor;
};
