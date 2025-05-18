// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiGameInstance.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "WorldObjects/Crack.h"
#include "Util/DebugHelper.h"

const int32 UDomiGameInstance::NumBosses = 1;

UDomiGameInstance::UDomiGameInstance()
	: RecentCrackIndex(0)
{
	IsBossDeadArray.Init(false, NumBosses);
	// PastCrackActivateArray.Init(false, 10);
	// PresentCrackActivateArray.Init(false, 10);
	CurrentGameStoryState = EGameStoryState::Tutorial;
}

void UDomiGameInstance::LoadSaveData(const FInstanceData& SaveData)
{
	IsBossDeadArray = SaveData.IsBossDeadArray;
	CurrentLevelName = SaveData.CurrentLevelName;
	CurrentLevelDisplayName = SaveData.CurrentLevelDisplayName;
	RecentCrackName = SaveData.RecentCrackName;
	RecentCrackIndex = SaveData.RecentCrackIndex;
}

FInstanceData UDomiGameInstance::GetSaveData() const
{
	FInstanceData SaveData;

	SaveData.IsBossDeadArray = IsBossDeadArray;
	SaveData.CurrentLevelName = CurrentLevelName;
	SaveData.CurrentLevelDisplayName = CurrentLevelDisplayName;
	SaveData.RecentCrackName = RecentCrackName;
	SaveData.RecentCrackIndex = RecentCrackIndex;

	return SaveData;
}

void UDomiGameInstance::SetIsBossDead(int32 BossIndex)
{
	check(IsBossDeadArray.IsValidIndex(BossIndex));
	IsBossDeadArray[BossIndex] = true;
}

bool UDomiGameInstance::GetIsBossDead(int32 BossIndex) const
{
	check(IsBossDeadArray.IsValidIndex(BossIndex));
	return IsBossDeadArray[BossIndex];
}

bool UDomiGameInstance::GetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) const
{
	if (const FCrackDataArray* CrackDataArrayRef = CrackDataMap.Find(LevelName))
	{
		Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : CrackDataMap not Founded"));
		if (CrackDataArrayRef->CrackDataArray.IsValidIndex(InCrackIndex))
		{
			Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : CrackDataMap is returned!!"));
			return CrackDataArrayRef->CrackDataArray[InCrackIndex].bIsActivate;
		}
	}

	Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : result is nullptr"));
	return false;
}

const FCrackData* UDomiGameInstance::GetCrackData(const FString& LevelName, int32 InCrackIndex) const
{
	if (const FCrackDataArray* CrackDataArrayRef = CrackDataMap.Find(LevelName))
	{
		if (CrackDataArrayRef->CrackDataArray.IsValidIndex(InCrackIndex))
		{
			return &CrackDataArrayRef->CrackDataArray[InCrackIndex];
		}
	}

	return nullptr;
}