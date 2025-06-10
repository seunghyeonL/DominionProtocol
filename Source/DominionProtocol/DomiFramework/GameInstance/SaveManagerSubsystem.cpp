// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManagerSubsystem.h"

#include "DomiGameInstance.h"
#include "ItemInstanceSubsystem.h"
#include "SoundInstanceSubsystem.h"
#include "WorldInstanceSubsystem.h"
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

	//ItemInstanceSubsystem 저장할 데이터 구조체(FItemInstanceSubsystem)로 Get
	UItemInstanceSubsystem* ItemInstanceSubsystem = GameInstance->GetSubsystem<UItemInstanceSubsystem>();
	if (IsValid(ItemInstanceSubsystem))
	{
		SaveGameInstance->ItemSubsystemData = ItemInstanceSubsystem->GetSaveData();
	}

	//WorldInstanceSubsystem 저장할 데이터 구조체(FWorldInstanceSubsystem)로 Get
	UWorldInstanceSubsystem* WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	if (IsValid(WorldInstanceSubsystem))
	{
		SaveGameInstance->WorldInstanceSubsystemData = WorldInstanceSubsystem->GetSaveData();
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

	//ItemInstanceSubsystem SaveData Load(FSoundSubsystemData)
	UItemInstanceSubsystem* ItemInstanceSubsystem = GameInstance->GetSubsystem<UItemInstanceSubsystem>();
	if (IsValid(ItemInstanceSubsystem))
	{
		ItemInstanceSubsystem->LoadSaveData(LoadedGame->ItemSubsystemData);
	}

	//WorldInstanceSubsystem SaveData Load(FSoundSubsystemData)
	UWorldInstanceSubsystem* WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	if (IsValid(WorldInstanceSubsystem))
	{
		WorldInstanceSubsystem->LoadSaveData(LoadedGame->WorldInstanceSubsystemData);
	}

	Debug::Print(TEXT("Success Load All Data "));
	return true;
}