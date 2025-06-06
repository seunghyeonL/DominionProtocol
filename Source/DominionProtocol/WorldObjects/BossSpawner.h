// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interface/StoryDependentInterface.h"
#include "WorldObjects/BossRoomDoor.h"
#include "BossSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossSpawnedSignature, FGameplayTag, BossTag, ABossRoomDoor*, BossDoor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossSpawnedToWidget, AActor*);

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

public:
	UPROPERTY(BlueprintAssignable)
	FOnBossSpawnedSignature OnBossSpawned;

	FOnBossSpawnedToWidget OnBossSpawnedToWidget;

	UPROPERTY(EditAnywhere)
	FGameplayTag BossTag;

	UPROPERTY(EditAnywhere)
	ABossRoomDoor* RelatedBossDoor;

protected:

	UPROPERTY(EditAnywhere, Category = "Boss")
	EGameStoryState RequiredStoryState;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AActor> BossClass;

	//UPROPERTY()
	//bool bHasSpawned = false;

	UPROPERTY()
	EGameStoryState CachedStoryState;
};
