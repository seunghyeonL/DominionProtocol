// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewSaveSlot.h"

#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"
#include "Player/TitleController.h"

void UNewSaveSlot::StartGame(const int32 SlotIndex) const
{
	// SlotIndex는 위젯에서 설정
	if (SlotIndex != -1)
	{
		auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
		check(TitleController);
		TitleController->FadeOut();
		
		FTimerHandle FadeTimerHandle;
		TWeakObjectPtr WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(
			FadeTimerHandle,
			[WeakThis, SlotIndex]()
			{
				if (WeakThis.IsValid())
				{
					check(WeakThis->GetSaveManagerSubsystemInstance());
					WeakThis->GetSaveManagerSubsystemInstance()->StartNewGame(SlotIndex);	
				}
			},
			TitleController->GetFadeDuration() + 0.1f,
			false
			);
	}
}

void UNewSaveSlot::LoadGame(const int32 SlotIndex) const
{
	if (SlotIndex != -1)
	{
		auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
		check(TitleController);
		TitleController->FadeOut();

		FTimerHandle FadeTimerHandle;
		TWeakObjectPtr WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(
			FadeTimerHandle,
			[WeakThis, SlotIndex]()
			{
				if (WeakThis.IsValid())
				{
					check(WeakThis->GetSaveManagerSubsystemInstance());
					WeakThis->GetSaveManagerSubsystemInstance()->LoadSaveDataAndOpenLevel(SlotIndex);	
				}
			},
			TitleController->GetFadeDuration() + 0.1f,
			false
			);
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
