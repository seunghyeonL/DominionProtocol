// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInstanceSubsystem.h"
#include "EnumAndStruct/FCrackData.h"

#include "Util/DebugHelper.h"

void UWorldInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DropEssenceCache = nullptr;
	bIsDropEssenceExist = false;
	DropEssenceAmount = 0;
	DropEssenceLocation = FVector::ZeroVector;
}

void UWorldInstanceSubsystem::Deinitialize()
{
	OnRecentCrackChanged.Clear();
	Super::Deinitialize();
}

void UWorldInstanceSubsystem::LoadSaveData(const FWorldInstanceSubsystemData& SaveData)
{
}

FWorldInstanceSubsystemData UWorldInstanceSubsystem::GetSaveData()
{
	FWorldInstanceSubsystemData SaveData;
	return SaveData;
}

void UWorldInstanceSubsystem::InitializeCrackDataMap(FCrackData Level1, FCrackData Level2)
{
	if (!CrackDataMap.Contains(Level1.OwnerLevelName))
	{
		FCrackDataArrayStruct Level1CrackArray;
		Level1CrackArray.CrackDataArray.Add(Level1);
		CrackDataMap.Add(Level1.OwnerLevelName, Level1CrackArray);
	}

	if (!CrackDataMap.Contains(Level2.OwnerLevelName))
	{
		FCrackDataArrayStruct Level2CrackArray;
		Level2CrackArray.CrackDataArray.Add(Level2);
		CrackDataMap.Add(Level2.OwnerLevelName, Level2CrackArray);
	}
}

void UWorldInstanceSubsystem::SetRecentCrackIndex(int32 NewRecentCrackIndex)
{
	RecentCrackIndex = NewRecentCrackIndex;

	OnRecentCrackChanged.Broadcast(NewRecentCrackIndex);
}

bool UWorldInstanceSubsystem::GetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) const
{
	if (const FCrackDataArrayStruct* CrackDataArrayRef = CrackDataMap.Find(LevelName))
	{
		Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : CrackDataMap Founded"));
		if (CrackDataArrayRef->CrackDataArray.IsValidIndex(InCrackIndex))
		{
			Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : CrackDataMap is returned!!"));
			return CrackDataArrayRef->CrackDataArray[InCrackIndex].bIsActivate;
		}
	}

	Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : result is nullptr"));
	return false;
}

const FCrackData* UWorldInstanceSubsystem::GetCrackData(const FString& LevelName, int32 InCrackIndex) const
{
	if (const FCrackDataArrayStruct* CrackDataArrayRef = CrackDataMap.Find(LevelName))
	{
		if (CrackDataArrayRef->CrackDataArray.IsValidIndex(InCrackIndex))
		{
			return &CrackDataArrayRef->CrackDataArray[InCrackIndex];
		}
	}

	return nullptr;
}
