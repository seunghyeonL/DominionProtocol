// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/MagicSkill/MagicCurvedProjectileSkill.h"
#include "Boss3RockThrow.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBoss3RockThrow : public UMagicCurvedProjectileSkill
{
	GENERATED_BODY()
public:
	UBoss3RockThrow();	

	virtual void Execute() override;
};

