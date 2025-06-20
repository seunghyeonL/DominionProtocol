// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Boss3Rock.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API ABoss3Rock : public ACurvedProjectile
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FRotator RotationPerSecond = FRotator(0, 0, 300.f);
};
