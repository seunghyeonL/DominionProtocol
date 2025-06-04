// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "ActorStateManageWorldSubsystem.generated.h"

struct FWorldActorData;

UCLASS()
class DOMINIONPROTOCOL_API UActorStateManageWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void UpdateActorDataMap(FGuid ID, FWorldActorData& Data);

	//GameMode에서 호출
	void UpdateWorldInstanceActorDataMap();
	//저장 전 플레이어 사망 시 정보 초기화
	FORCEINLINE void ClearActorDataMap() { ActorDataMap.Empty(); }
	
	FORCEINLINE void AddActorData(FGuid ID, FWorldActorData& Data)
	{
		if (ActorDataMap.Contains(ID))
		{
			return;
		}
		ActorDataMap.Add(ID, Data);
	}

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	UPROPERTY()
	TMap<FGuid, FWorldActorData> ActorDataMap;

	UPROPERTY()
	TObjectPtr<UWorldInstanceSubsystem> WorldInstanceSubsystem;
};
