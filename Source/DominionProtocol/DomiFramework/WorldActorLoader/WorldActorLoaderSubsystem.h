// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/ECategory.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnumAndStruct/FWorldActorData.h"
#include "WorldActorLoaderSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UWorldActorLoaderSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void SpawnActorToTargetTransform(TSubclassOf<AActor> ActorClass, EWorldActorCategory ActorCategory, FGuid InUniqueID, const FVector& Location, const FRotator& Rotation);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	TMap<FGuid, FWorldItemData> ItemDataMap;
	TMap<FGuid, FWorldInteractableData> InteractableDataMap;
	TMap<FGuid, FWorldEnemyData> EnemyDataMap;
};
