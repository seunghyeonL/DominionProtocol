// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheatBPLib.generated.h"

class ADomiCharacter;
class UStatusComponent;
class ABaseGameMode;
class UItemComponent;

UCLASS()
class DOMINIONPROTOCOL_API UCheatBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void Save(const UWorld* World, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	static void Load(const UWorld* World, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	static void StartGameNewSlot(const UWorld* World, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	static void SaveSlotLoadAndStartGame(const UWorld* World, int32 SlotIndex);
	
	static void InfiniteStamina(const TObjectPtr<UStatusComponent> StatusComponent);

	UFUNCTION(BlueprintCallable)
	static void MoveToCrack(ABaseGameMode* BaseGameMode, FString TargetLevelName, int32 TargetCrackIndex);

	UFUNCTION(BlueprintCallable)
	static void ForceMoveToCrack(UWorld* World, int32 TargetCrackIndex, ADomiCharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	static void AddAllItemsToPlayerInventoryMaxQuantity(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void EquipSword(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ToggleFog(UObject* WorldContextObject);
};
