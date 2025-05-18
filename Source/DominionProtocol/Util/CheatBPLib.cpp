// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatBPLib.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"

#include "Util/DebugHelper.h"

void UCheatBPLib::Save(const UWorld* World)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (SaveSubsystem->SaveGame("SaveGame1", 0))
	{
		Debug::Print("Save Game Success");
	}

	UWorldInstanceSubsystem* WorldInstanceSubsystem = World->GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	Debug::Print(TEXT("Save Game in SaveGame1"));
	Debug::Print(FString::Printf(TEXT("SaveData[CurrentLevelName] : %s"), *WorldInstanceSubsystem->GetCurrentLevelName()));
}

void UCheatBPLib::InfiniteStamina(const TObjectPtr<UStatusComponent> StatusComponent)
{
	if (IsValid(StatusComponent))
	{
		StatusComponent->SwitchInfiniteStaminaMode();
	}
}

void UCheatBPLib::MoveToCrack(ABaseGameMode* BaseGameMode, FString TargetLevelName, int32 TargetCrackIndex)
{
	BaseGameMode->MoveToTargetCrack(TargetLevelName, TargetCrackIndex);
}
