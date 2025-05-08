// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "ProtoLevel2GameMode.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY()
	FTransform OriginTransform;
};

UCLASS()
class DOMINIONPROTOCOL_API AProtoLevel2GameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	AProtoLevel2GameMode();

	//균열 쪽에서 호출할 함수
	void OnCrackUsed();	// 기존 적들 제거
	void RespawnEnemies();	// 적 기존 위치에 리스폰
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TArray<FEnemySpawnInfo> CachedEnemyInfo;
};
