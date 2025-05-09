// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatBPLib.h"

#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"

#include "Util/DebugHelper.h"

void UCheatBPLib::Save(const UWorld* World)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	SaveSubsystem->SaveGame("SaveGame1.sav", 0);

	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(World->GetGameInstance());
	Debug::Print(TEXT("Save Game in SaveGame1.sav"));
	Debug::Print(FString::Printf(TEXT("SaveData[CurrentLevelName] : %s"), *GameInstance->GetCurrentLevelName()));
}
