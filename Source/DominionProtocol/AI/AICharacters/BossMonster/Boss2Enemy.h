// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "Boss2Enemy.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API ABoss2Enemy : public ABaseBossEnemy
{
	GENERATED_BODY()
	
public:
	ABoss2Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
