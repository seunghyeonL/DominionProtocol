// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interface/StoryDependentInterface.h"
#include "BossSpawner.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABossSpawner : public AActor, public IStoryDependentInterface
{
	GENERATED_BODY()
	
public:	
	ABossSpawner();

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SpawnBoss();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnStoryStateUpdated_Implementation(EGameStoryState NewState);

protected:

	UPROPERTY(EditAnywhere, Category = "Boss")
	EGameStoryState RequiredStoryState;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AActor> BossClass;

	UPROPERTY(EditAnywhere, Category = "Boss")
	FGameplayTag BossTag;

	UPROPERTY()
	bool bHasSpawned = false;

	UPROPERTY()
	EGameStoryState CachedStoryState;
};
