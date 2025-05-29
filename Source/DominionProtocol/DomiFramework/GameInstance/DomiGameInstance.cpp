// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiGameInstance.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "WorldObjects/Crack.h"
#include "Util/DebugHelper.h"

UDomiGameInstance::UDomiGameInstance()
{
	CurrentGameStoryState = EGameStoryState::Tutorial;
}

void UDomiGameInstance::LoadSaveData(const FInstanceData& SaveData)
{
	// DeadBossTags = SaveData.DeadBossTags;
	// CurrentLevelName = SaveData.CurrentLevelName;
	// CurrentLevelDisplayName = SaveData.CurrentLevelDisplayName;
	// RecentCrackName = SaveData.RecentCrackName;
	// RecentCrackIndex = SaveData.RecentCrackIndex;
	// CurrentGameStoryState = SaveData.CurrentGameStoryState;
}

FInstanceData UDomiGameInstance::GetSaveData() const
{
	FInstanceData SaveData;
	// SaveData.DeadBossTags = DeadBossTags;
	// SaveData.CurrentLevelName = CurrentLevelName;
	// SaveData.CurrentLevelDisplayName = CurrentLevelDisplayName;
	// SaveData.RecentCrackName = RecentCrackName;
	// SaveData.RecentCrackIndex = RecentCrackIndex;
	// SaveData.CurrentGameStoryState = CurrentGameStoryState;
	return SaveData;
}

void UDomiGameInstance::SetIsBossDead(FGameplayTag BossTag)
{
	if (BossTag.IsValid())
	{
		DeadBossTags.Add(BossTag);
	}
}

void UDomiGameInstance::SetCurrentGameStoryState(EGameStoryState NewGameStoryState)
{
	if (CurrentGameStoryState != NewGameStoryState)
	{
		Debug::Print(FString::Printf(TEXT("AdvanceStoryState called → %d"), NewGameStoryState));
		CurrentGameStoryState = NewGameStoryState;
		OnStoryStateChanged.Broadcast(NewGameStoryState);
	}
}

bool UDomiGameInstance::GetIsBossDead(FGameplayTag BossTag) const
{
	return DeadBossTags.Contains(BossTag);
}

void UDomiGameInstance::AdvanceStoryState()
{
	int32 CurrentIndex = static_cast<int32>(CurrentGameStoryState);
	EGameStoryState NextState = static_cast<EGameStoryState>(CurrentIndex + 1);

	if (UEnum* EnumPtr = StaticEnum<EGameStoryState>())
	{
		int32 MaxIndex = EnumPtr->NumEnums() - 1;
		if (CurrentIndex + 1 < MaxIndex)
		{
			SetCurrentGameStoryState(NextState);
			Debug::Print(FString::Printf(TEXT("GameInstance - AdvanceStoryState: [%d] → [%d]"), CurrentIndex, static_cast<int32>(NextState)));
		}
		else
		{
			Debug::Print(TEXT("GameInstance - AdvanceStoryState: Already at last state."));
		}
	}
}
