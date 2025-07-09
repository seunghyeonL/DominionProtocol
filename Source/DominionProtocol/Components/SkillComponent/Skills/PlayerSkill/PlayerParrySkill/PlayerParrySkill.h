// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "PlayerParrySkill.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerParrySkill : public UBaseSkill
{
	GENERATED_BODY()
public:
	UPlayerParrySkill();

	virtual void Execute() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Parry")
	float ParryDuration = 0.3f;
};
