// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiGameInstance.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "WorldObjects/Crack.h"
#include "Util/DebugHelper.h"

void UDomiGameInstance::Init()
{
	Super::Init();

	CurrentGameStoryState = EGameStoryState::Tutorial;
	PlayerCurrentEssence = 0;
}

void UDomiGameInstance::LoadSaveData(const FInstanceData& SaveData)
{
	PlayerCurrentEssence = SaveData.PlayerCurrentEssence;
	StatDataMap = SaveData.StatDataMap;
}

FInstanceData UDomiGameInstance::GetSaveData() const
{
	FInstanceData SaveData;
	SaveData.StatDataMap = StatDataMap;
	SaveData.PlayerCurrentEssence = PlayerCurrentEssence;
	return SaveData;
}

void UDomiGameInstance::ApplySaveData()
{
	if (IsValid(World))
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(World->GetFirstPlayerController()->GetPawn());
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->GetStatusComponent()->SetStatMap(StatDataMap);
		}
	}
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

bool UDomiGameInstance::ReturnStoryState()
{
	switch (CurrentGameStoryState)
	{
	case EGameStoryState::Battle_Boss1:
		SetCurrentGameStoryState(EGameStoryState::Find_Boss1);
		break;

	case EGameStoryState::Battle_Boss2:
		SetCurrentGameStoryState(EGameStoryState::Find_Boss2);
		break;

	case EGameStoryState::Battle_Boss3_1:
		SetCurrentGameStoryState(EGameStoryState::Find_Boss3_1);
		break;

	case EGameStoryState::Battle_Boss3_2:
		SetCurrentGameStoryState(EGameStoryState::Find_Boss3_2);
		break;

	case EGameStoryState::Battle_Boss4:
		SetCurrentGameStoryState(EGameStoryState::Find_Boss4);
		break;

	default:
		return false;
	}

	Debug::Print(FString::Printf(TEXT("GameInstance - ReturnStoryState: [%d] → [%d]"),
		static_cast<int32>(CurrentGameStoryState),
		static_cast<int32>(GetCurrentGameStoryState())));

	return true;
}
