// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheatBPLib.generated.h"

class UStatusComponent;
class ABaseGameMode;
class UItemComponent;

UCLASS()
class DOMINIONPROTOCOL_API UCheatBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void Save(const UWorld* World);

	static void InfiniteStamina(const TObjectPtr<UStatusComponent> StatusComponent);

	UFUNCTION(BlueprintCallable)
	static void MoveToCrack(ABaseGameMode* BaseGameMode, FString TargetLevelName, int32 TargetCrackIndex);

	UFUNCTION(BlueprintCallable)
	static void AddAllItemsToPlayerInventoryMaxQuantity(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void ToggleFog(UWorld* World);
};
