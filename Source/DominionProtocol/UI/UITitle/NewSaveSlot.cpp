// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewSaveSlot.h"

#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"

void UNewSaveSlot::StartGame(const int32 SlotIndex) const
{
	// SlotIndex는 위젯에서 설정
	if (SlotIndex != -1)
	{
		SaveManagerSubsystemInstance->StartNewGame(SlotIndex);	
	}
}

void UNewSaveSlot::LoadGame(const int32 SlotIndex) const
{
	if (SlotIndex != -1)
	{
		SaveManagerSubsystemInstance->LoadSaveDataAndOpenLevel(SlotIndex);	
	}
}

void UNewSaveSlot::DeleteGame(const int32 SlotIndex) const
{
	if (SlotIndex != -1)
	{
		SaveManagerSubsystemInstance->DeleteSaveSlot(SlotIndex);
	}
}

void UNewSaveSlot::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* GameInstance = GetWorld()->GetGameInstance();
	auto* SaveManagerSubsystem = GameInstance->GetSubsystem<USaveManagerSubsystem>();
	
	ensureMsgf(SaveManagerSubsystem, TEXT("Casting failed to SoundSubsystem in UNewSettingWidget"));
	
	if (SaveManagerSubsystem)
	{
		SaveManagerSubsystemInstance = SaveManagerSubsystem;
	}
}
