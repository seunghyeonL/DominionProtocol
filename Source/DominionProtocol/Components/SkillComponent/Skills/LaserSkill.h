// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkill.h"
#include "LaserSkill.generated.h"

class UMnhCapsuleComponent;
class ALaserActor;

UCLASS()
class DOMINIONPROTOCOL_API ULaserSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	ULaserSkill();

	virtual void Execute() override;

	virtual void StartTrace() override;
	virtual void StopTrace() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<ALaserActor> LaserActor;
};
