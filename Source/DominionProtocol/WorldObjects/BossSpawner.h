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

	void UpdateFade();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag BossTag;

	FOnCreateDialogueManager OnCreateDialogueManager;

protected:
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<AActor> BossClass;

	FTimerHandle FadeTimerHandle;
	float FadeElapsedTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "FadeEffect")
	float FadeDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "FadeEffect")
	UMaterialParameterCollection* FadeMPC;

	UMaterialParameterCollectionInstance* MPCInstance;

	UPROPERTY(EditAnywhere, Category = "FadeEffect")
	UCurveFloat* FadeCurve;
};
