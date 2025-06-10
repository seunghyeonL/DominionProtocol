// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "BossSpawner.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABossSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossSpawner();

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SpawnBoss();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag BossTag;

protected:
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AActor> BossClass;
};
