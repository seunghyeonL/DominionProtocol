// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumAndStruct/FItemSubsystemData.h"

#include "Util/GameTagList.h"

#include "ItemInstanceSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UItemInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Share
	
public:
	//Save & Load
	void LoadSaveData(const FItemSubsystemData& SaveData);
	FItemSubsystemData GetSaveData() const;
	
	// Setter
	FORCEINLINE void SetWorldCache(UWorld* InWorld) { World = InWorld; }

	FORCEINLINE void SetInventoryDataMap(const TMap<FGameplayTag, int32>& NewInventoryMap) { InventoryDataMap = NewInventoryMap; }

	FORCEINLINE void SetEquipmentSlotMap(const TMap<FName, FGameplayTag>& NewEquipMap) { EquipmentSlotMap = NewEquipMap; }

	FORCEINLINE void SetConsumableSlotMap(const TMap<FName, FGameplayTag>& NewConsumableMap) { ConsumableSlotMap = NewConsumableMap; }
	
	FORCEINLINE void SetPotionBoostLevel(int32 NewInt) { PotionBoostLevel = NewInt; }

	// Getter
	FORCEINLINE TMap<FGameplayTag, int32>& GetInventoryDataMap() { return InventoryDataMap; }

	FORCEINLINE TMap<FName, FGameplayTag>& GetEquipmentSlotMap() { return EquipmentSlotMap; }

	FORCEINLINE TMap<FName, FGameplayTag>& GetConsumableSlotMap() {	return ConsumableSlotMap; }

	FORCEINLINE int32 GetPotionBoostLevel() { return PotionBoostLevel; }
	
protected:
	virtual auto Initialize(FSubsystemCollectionBase& Collection) -> void override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TMap<FGameplayTag, int32> InventoryDataMap;

	UPROPERTY()
	TMap<FName, FGameplayTag> EquipmentSlotMap;

	UPROPERTY()
	TMap<FName, FGameplayTag> ConsumableSlotMap;

	UPROPERTY()
	int32 PotionBoostLevel;

	//Not Save
	UPROPERTY()
	TObjectPtr<UWorld> World;
	
	
#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
