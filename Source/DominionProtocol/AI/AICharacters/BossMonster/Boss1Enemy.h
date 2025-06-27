// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "Boss1Enemy.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API ABoss1Enemy : public ABaseBossEnemy
{
	GENERATED_BODY()
	
public:
	ABoss1Enemy();

	virtual void OnDeath_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnDropItem() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnGroggy() override;
};
