// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiGameInstance.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "WorldObjects/Crack.h"
#include "Util/DebugHelper.h"

const int32 UDomiGameInstance::NumBosses = 5;

UDomiGameInstance::UDomiGameInstance()
{
	IsBossDeadArray.Init(false, NumBosses);
	
	CurrentGameStoryState = EGameStoryState::Tutorial;
}

void UDomiGameInstance::LoadSaveData(const FInstanceData& SaveData)
{
	IsBossDeadArray = SaveData.IsBossDeadArray;
	// CurrentLevelName = SaveData.CurrentLevelName;
	// CurrentLevelDisplayName = SaveData.CurrentLevelDisplayName;
	// RecentCrackName = SaveData.RecentCrackName;
	// RecentCrackIndex = SaveData.RecentCrackIndex;
}

FInstanceData UDomiGameInstance::GetSaveData() const
{
	FInstanceData SaveData;

	// SaveData.IsBossDeadArray = IsBossDeadArray;
	// SaveData.CurrentLevelName = CurrentLevelName;
	// SaveData.CurrentLevelDisplayName = CurrentLevelDisplayName;
	// SaveData.RecentCrackName = RecentCrackName;
	// SaveData.RecentCrackIndex = RecentCrackIndex;

	return SaveData;
}

void UDomiGameInstance::SetIsBossDead(int32 BossIndex)
{
	check(IsBossDeadArray.IsValidIndex(BossIndex));
	IsBossDeadArray[BossIndex] = true;
}

void UDomiGameInstance::SetCurrentGameStoryState(EGameStoryState NewGameStoryState)
{
	if (CurrentGameStoryState != NewGameStoryState)
	{
		CurrentGameStoryState = NewGameStoryState;
		OnStoryStateChanged.Broadcast(NewGameStoryState);
	}
}

bool UDomiGameInstance::GetIsBossDead(int32 BossIndex) const
{
	check(IsBossDeadArray.IsValidIndex(BossIndex));
	return IsBossDeadArray[BossIndex];
}