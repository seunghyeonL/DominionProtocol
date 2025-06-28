// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "WorldObjects/DialogueManager.h"
#include "BossSpawner.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateDialogueManager, UDialogueManager*);

UCLASS()
class DOMINIONPROTOCOL_API ABossSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossSpawner();

	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SpawnBoss();

	UFUNCTION()
	void ResetFade(AActor* Boss);

	void UpdateReverseFade();

protected:
	virtual void BeginPlay() override;

	void StartFade();
	void UpdateFade();

	void SpawnBossInternal();

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag BossTag;

	FOnCreateDialogueManager OnCreateDialogueManager;

protected:
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AActor> BossClass;

	UPROPERTY()
	TWeakObjectPtr<AActor> SpawnedBoss;

	FTimerHandle FadeTimerHandle;
	FTimerHandle PreFadeDelayHandle;

	float FadeElapsedTime = 0.0f; // 현재까지 경과한 시간

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	float FadeDuration = 5.0f; // 메테리얼 변화 연출이 걸리는 시간

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	float Boss2SpawnDelay = 5.0f; // 보스2 페이드 시작 전 대기 시간

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	UMaterialParameterCollection* FadeMPC;

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	UMaterialParameterCollection* FadeMPC2; // 보스4 2페이즈 

	UMaterialParameterCollectionInstance* MPCInstance;
	UMaterialParameterCollectionInstance* MPCInstance2;

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	UCurveFloat* FadeCurve;
};
