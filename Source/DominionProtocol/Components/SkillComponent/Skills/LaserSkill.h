// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkill.h"
#include "LaserSkill.generated.h"

class UMnhCapsuleComponent;

UCLASS()
class DOMINIONPROTOCOL_API ULaserSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	ULaserSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) override;

	virtual void BeginDestroy() override;

protected:
	UPROPERTY()
	TObjectPtr<UMnhCapsuleComponent> CapsuleComponent;
};
