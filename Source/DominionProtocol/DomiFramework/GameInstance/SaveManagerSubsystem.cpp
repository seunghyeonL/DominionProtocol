// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManagerSubsystem.h"

#include "DomiGameInstance.h"
#include "SoundInstanceSubsystem.h"
#include "DomiFramework/DomiSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "Util/DebugHelper.h"

bool USaveManagerSubsystem::SaveGame(const FString& SlotName, int32 UserIndex)
{
	//저장 인스턴스 생성
	UDomiSaveGame* SaveGameInstance = Cast<UDomiSaveGame>(UGameplayStatics::CreateSaveGameObject(UDomiSaveGame::StaticClass()));
	check(SaveGameInstance);

	//DomiGameInstance 저장할 데이터 구조체(FInstanceData)로 Get
	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		SaveGameInstance->InstanceData = GameInstance->GetSaveData();
	}

	//SoundInstanceSubsystem 저장할 데이터 구조체(FSoundSubsystem)로 Get
	USoundInstanceSubsystem* SoundSubsystem = GameInstance->GetSubsystem<USoundInstanceSubsystem>();
	if (IsValid(SoundSubsystem))
	{
		SaveGameInstance->SoundSubsystemData = SoundSubsystem->GetSaveData();
	}

	//저장 실행
	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);
}

bool USaveManagerSubsystem::LoadGame(const FString& SlotName, int32 UserIndex)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		Debug::PrintError(TEXT("Invalid SaveSlot"));
		return false;
	}

	UDomiSaveGame* LoadedGame = Cast<UDomiSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!IsValid(LoadedGame))
	{
		Debug::PrintError(TEXT("Load GameData Failed"));
		return false;
	}

	//DomiGameInstance SaveData Load(FInstanceData)
	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		GameInstance->LoadSaveData(LoadedGame->InstanceData);
	}

	//SoundInstanceSubsystem SaveData Load(FSoundSubsystemData)
	USoundInstanceSubsystem* SoundSubsystem = GameInstance->GetSubsystem<USoundInstanceSubsystem>();
	if (IsValid(SoundSubsystem))
	{
		SoundSubsystem->LoadSaveData(LoadedGame->SoundSubsystemData);
	}

	Debug::Print(TEXT("Success Load All Data "));
	return true;
}