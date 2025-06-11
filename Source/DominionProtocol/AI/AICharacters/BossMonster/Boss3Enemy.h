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
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage2;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage3;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage4;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack2();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack3();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack4();

	UPROPERTY(EditAnywhere, Category = "Throw Pattern")
	UStaticMesh* StoneMesh;

	UPROPERTY()
	UStaticMeshComponent* SpawnedStoneMeshComp;

	FTimerHandle StoneSpawnDelayHandle;
	FTimerHandle StoneDestroyHandle;

	void SpawnStone();
};
