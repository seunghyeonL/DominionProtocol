// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "NomalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API ANomalEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	virtual void OnDeath_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	bool bUsePatrolPoint = false;

	int32 CurrentPatrolIndex = 0;

	void BeginPlay() override;
};
