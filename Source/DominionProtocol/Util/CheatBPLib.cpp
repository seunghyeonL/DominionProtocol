// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatBPLib.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"

#include "Util/DebugHelper.h"

void UCheatBPLib::Save(const UWorld* World)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (SaveSubsystem->SaveGame("SaveGame1", 0))
	{
		Debug::Print("Save Game Success");
	}

	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(World->GetGameInstance());
	Debug::Print(TEXT("Save Game in SaveGame1"));
	Debug::Print(FString::Printf(TEXT("SaveData[CurrentLevelName] : %s"), *GameInstance->GetCurrentLevelName()));
}

void UCheatBPLib::InfiniteStamina(const TObjectPtr<UStatusComponent> StatusComponent)
{
	if (IsValid(StatusComponent))
	{
		StatusComponent->SwitchInfiniteStaminaMode();
	}
}
