// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "BaseGameMode.generated.h"

class ADomiCharacter;
class ACrack;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY()
	FTransform OriginTransform;
};

DECLARE_MULTICAST_DELEGATE(FOnPlayerSpawn);

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABaseGameMode();

	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void StartBattle();
	virtual void EndBattle();

	virtual void OnPlayerDeath();
	
	void RespawnPlayerCharacter();

	//균열 쪽에서 호출할 함수
	void DestroyAllNormalEnemy();	// 기존 적들 제거
	void RespawnEnemies();	// 적 기존 위치에 리스폰

	//Getter
	
	//Setter
	FORCEINLINE void SetRecentCrackCache(ACrack* NewCrack) { RecentCrackCache = NewCrack ; }

public:
	// Delegate
	FOnPlayerSpawn OnPlayerSpawn;

protected:
	UPROPERTY()
	UDomiGameInstance* GameInstance;

	UPROPERTY()
	ADomiCharacter* PlayerCharacter;
	
	UPROPERTY()
	ACrack* RecentCrackCache;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RespawnDelay;

	UPROPERTY()
	TArray<FEnemySpawnInfo> CachedEnemyInfo;
	
	FString CurrentLevel;
	
};
