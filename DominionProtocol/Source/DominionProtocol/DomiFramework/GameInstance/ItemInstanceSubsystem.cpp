// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInstanceSubsystem.h"

#include "Util/DebugHelper.h"

void UItemInstanceSubsystem::LoadSaveData(const FItemSubsystemData& SaveData)
{
}

FItemSubsystemData UItemInstanceSubsystem::GetSaveData() const
{
	FItemSubsystemData SaveData;

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
