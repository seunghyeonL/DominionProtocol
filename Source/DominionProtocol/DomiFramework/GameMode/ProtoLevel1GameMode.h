// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Engine/TriggerBox.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "Engine/TargetPoint.h"
#include "ProtoLevel1GameMode.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API AProtoLevel1GameMode : public ABaseGameMode
{
	GENERATED_BODY()

	
protected:
	virtual void StartPlay() override;

	UFUNCTION()
	void EndBattle() override;

	virtual void OnPlayerDeath() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	TSubclassOf<ABaseEnemy> BossClass;

	UPROPERTY(EditAnywhere, Category = "Boss")
	ATargetPoint* BossSpawnPoint;
};
