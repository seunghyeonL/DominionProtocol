// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInstanceSubsystem.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/ItemComponent/ItemComponent.h"

#include "Util/DebugHelper.h"

void UItemInstanceSubsystem::LoadSaveData(const FItemSubsystemData& SaveData)
{
	InventoryDataMap = SaveData.InventoryMap;
	EquipmentSlotMap = SaveData.EquipmentSlots;
	ConsumableSlotMap = SaveData.ConsumableSlots;
	bIsPotionBoostApplied = SaveData.bIsPotionBoostApplied;
}

FItemSubsystemData UItemInstanceSubsystem::GetSaveData() const
{
	FItemSubsystemData SaveData;

	SaveData.InventoryMap = InventoryDataMap;
	SaveData.EquipmentSlots = EquipmentSlotMap;
	SaveData.ConsumableSlots = ConsumableSlotMap;
	SaveData.bIsPotionBoostApplied = bIsPotionBoostApplied;
	
	return SaveData;
}

void UItemInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UItemInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
