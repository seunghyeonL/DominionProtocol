// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolableObjectInterface.h"
#include "PoolableActorBase.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API APoolableActorBase : public AActor, public IPoolableObjectInterface
{
	GENERATED_BODY()

	// 기본적으로 Tick은 false로 해놨고,
	// Mesh나 SkelemtalMesh의 bReceivesDecals도 false로 해놨습니다
public:
	APoolableActorBase();
	
	virtual void BeginPlay() override;
	
	void OnObjectSpawn_Implementation() override;

	void OnObjectReturn_Implementation() override;

protected:
	bool bIsActivate = false;
};
