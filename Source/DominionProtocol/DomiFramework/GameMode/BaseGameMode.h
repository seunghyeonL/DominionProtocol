// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "GameFramework/GameMode.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"
#include "BaseGameMode.generated.h"

class UItemComponent;
class ADropEssence;
class ALevelSequenceActor;
class ULevelSequencePlayer;
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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossSpawnedToWidget, AActor*);

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

#pragma region Share
	
public:
	ABaseGameMode();

	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	
	virtual void StartBattle(AActor* SpawnedBoss);
	virtual void EndBattle();

	virtual void OnPlayerDeath();

	UFUNCTION()
	void SaveItemDataToInstance();

	void UpdateInstanceData();
	
	void Save();
	
	void RestorePlayer();
	
	void RespawnPlayerCharacter();
	
	//UI 쪽에서 레벨과 균열인덱스를 정하면 해당 함수를 호출하도록 하면 됩니다
	UFUNCTION(BlueprintCallable)
	void MoveToTargetCrack(FString InOwningCrackLevelName, int32 InCrackIndex);
	
	//균열 쪽에서 호출할 함수
	void DestroyAllNormalEnemy();	// 기존 적들 제거
	void RespawnEnemies();	// 적 기존 위치에 리스폰

	//Getter
	FORCEINLINE int32 GetPlayTime() { return PlayTime; }
	
	//Setter
	FORCEINLINE void SetRecentCrackCache(ACrack* NewCrack) { RecentCrackCache = NewCrack ; }

protected:
	void PlayTimeAdder();
	
public:
	// Delegate
	FOnPlayerSpawn OnPlayerSpawn;
	FOnBossSpawnedToWidget OnBossSpawnedToWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ADropEssence> DropEssenceClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAudioComponent> EnterAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAudioComponent> ExitAudioComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundWave> EnterSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundWave> ExitSound;
	
	UPROPERTY()
	TObjectPtr<UDomiGameInstance> GameInstance;

	UPROPERTY()
	TObjectPtr<UWorldInstanceSubsystem> WorldInstanceSubsystem;

	UPROPERTY()
	TObjectPtr<UItemInstanceSubsystem> ItemInstanceSubsystem;

	UPROPERTY()
	TObjectPtr<ABaseGameState> BaseGameState;
	
	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<ADomiCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UStatusComponent> StatusComponent;

	UPROPERTY()
	TObjectPtr<UItemComponent> ItemComponent;
	
	UPROPERTY()
	TObjectPtr<ACrack> RecentCrackCache;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RespawnDelay;

	UPROPERTY()
	TArray<FEnemySpawnInfo> CachedEnemyInfo;

	bool bIsSameLevelMove;

	FName MoveTargetLevelName;
	
	FVector PendingMoveLocation;
	
	FRotator PendingMoveRotation;

	int32 PlayTime;

	FTimerHandle PlayTimer;

#pragma endregion

	
#pragma region KyuHyeok

public:
	UFUNCTION(BlueprintCallable)
	void PlayFade(bool bFadeIn);
	
	void PlayerLevelUp(FGameplayTag StatTag);
	
	void SetPlayerInputEnable(bool bEnable);
	
protected:
	UFUNCTION()
	void OnFadeSequenceFinished();
	
	UPROPERTY()
	TObjectPtr<ULevelSequence> FadeSequence;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;

	UPROPERTY()
	ALevelSequenceActor* SequenceActor;

	UPROPERTY()
	bool bIsFadeIn;
	
#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
