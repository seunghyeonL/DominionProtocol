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
	IsNewGameDataMap = SaveData.IsNewGameDataMap;
	CurrentLevelName = SaveData.CurrentLevelName;
	CurrentLevelDisplayName = SaveData.CurrentLevelDisplayName;
	RecentCrackName = SaveData.RecentCrackName;
	RecentCrackIndex = SaveData.RecentCrackIndex;
	MaxCrackIndex = SaveData.MaxCrackIndex;
	CrackDataMap = SaveData.CrackDataMap;
	WorldActorDataMap = SaveData.WorldActorDataMap;
	bIsDropEssenceExist = SaveData.bIsDropEssenceExist;
	DropEssenceAmount = SaveData.DropEssenceAmount;
	DropEssenceLocation = SaveData.DropEssenceLocation;
	DropEssenceLocationLevel = SaveData.DropEssenceLocationLevel;
}

FWorldInstanceSubsystemData UWorldInstanceSubsystem::GetSaveData()
{
	FWorldInstanceSubsystemData SaveData;

	SaveData.IsNewGameDataMap = IsNewGameDataMap;
	SaveData.CurrentLevelName = CurrentLevelName;
	SaveData.CurrentLevelDisplayName = CurrentLevelDisplayName;
	SaveData.RecentCrackName = RecentCrackName;
	SaveData.RecentCrackIndex = RecentCrackIndex;
	SaveData.MaxCrackIndex = MaxCrackIndex;
	SaveData.CrackDataMap = CrackDataMap;
	SaveData.WorldActorDataMap = WorldActorDataMap;
	SaveData.bIsDropEssenceExist = bIsDropEssenceExist;
	SaveData.DropEssenceAmount = DropEssenceAmount;
	SaveData.DropEssenceLocation = DropEssenceLocation;
	SaveData.DropEssenceLocationLevel = DropEssenceLocationLevel;
	
	return SaveData;
}

void UWorldInstanceSubsystem::InitializeCrackAndNewGameDataMap(FCrackData Level1, FCrackData Level2)
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

	if (!IsNewGameDataMap.Contains(Level1.OwnerLevelName))
	{
		IsNewGameDataMap.Add(Level1.OwnerLevelName, true);
	}

	if (!IsNewGameDataMap.Contains(Level2.OwnerLevelName))
	{
		IsNewGameDataMap.Add(Level2.OwnerLevelName, true);
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
