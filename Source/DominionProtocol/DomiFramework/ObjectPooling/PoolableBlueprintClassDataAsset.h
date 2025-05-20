// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PoolableBlueprintClassDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FPoolableActorBlueprintData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClass;
};

UCLASS()
class DOMINIONPROTOCOL_API UPoolableBlueprintClassDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FPoolableActorBlueprintData> PoolableBlueprintClassArray;
};
