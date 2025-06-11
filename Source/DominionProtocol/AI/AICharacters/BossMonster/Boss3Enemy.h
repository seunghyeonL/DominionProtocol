// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "Boss3Enemy.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API ABoss3Enemy : public ABaseBossEnemy
{
	GENERATED_BODY()
public:
	ABoss3Enemy();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();
};
