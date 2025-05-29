// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "WorldObjects/BossRoomDoor.h"
#include "BaseGameMode.generated.h"

class UStatusComponent;
class UItemInstanceSubsystem;
class UWorldInstanceSubsystem;
class ABaseGameState;
class ADomiCharacter;
class ACrack;
struct FGameplayTag;

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

#pragma region Share
	
public:
	ABaseGameMode();

	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	
	virtual void StartBattle(FGameplayTag BossTag, ABossRoomDoor* BossDoor);
	virtual void EndBattle();

	virtual void OnPlayerDeath();
	
	void RespawnPlayerCharacter();
	
	//UI 쪽에서 레벨과 균열인덱스를 정하면 해당 함수를 호출하도록 하면 됩니다
	UFUNCTION(BlueprintCallable)
	void MoveToTargetCrack(FString InOwningCrackLevelName, int32 InCrackIndex);
	
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
	UWorldInstanceSubsystem* WorldInstanceSubsystem;

	UPROPERTY()
	UItemInstanceSubsystem* ItemInstanceSubsystem;

	UPROPERTY()
	ABaseGameState* BaseGameState;
	
	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	ADomiCharacter* PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UStatusComponent> StatusComponent;
	
	UPROPERTY()
	ACrack* RecentCrackCache;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RespawnDelay;

	UPROPERTY()
	TArray<FEnemySpawnInfo> CachedEnemyInfo;

#pragma endregion

	
#pragma region KyuHyeok

public:
	void PlayerLevelUp(FGameplayTag StatTag);
	
#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
