// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewSaveSlotBoxUI.h"

#include "NewSaveSlot.h"
#include "Components/VerticalBox.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"
#include "Player/TitleController.h"

void UNewSaveSlotBoxUI::RefreshSlotData(const ESlateVisibility VisibilityState)
{
	if (VisibilityState == ESlateVisibility::Visible)
	{
		for (UNewSaveSlot* SaveSlot : SaveSlots)
		{
			SaveSlot->SetSaveSlotInfo();
		}	
	}
	else
	{
		CurrentSaveSlotBoxFocusIndex = -1;
	}
}

void UNewSaveSlotBoxUI::ChangeSaveSlotBoxFocusIndex(const int32 NewFocusIndex)
{
	for (int32 i = 0; i < SaveSlots.Num(); i++)
	{
		if (i == NewFocusIndex)
		{
			SaveSlots[i]->GetFocus();
			CurrentSaveSlotBoxFocusIndex = NewFocusIndex;
		}
		else
		{
			SaveSlots[i]->LoseFocus();
		}
	}
}

void UNewSaveSlotBoxUI::IncreaseSaveSlotBoxFocusIndex()
{
	if (CurrentSaveSlotBoxFocusIndex >= MaxSaveSlotBoxFocusIndex)
	{
		return;
	}

	ChangeSaveSlotBoxFocusIndex(CurrentSaveSlotBoxFocusIndex + 1);
}

void UNewSaveSlotBoxUI::DecreaseSaveSlotBoxFocusIndex()
{
	if (CurrentSaveSlotBoxFocusIndex <= 0)
	{
		return;
	}

	ChangeSaveSlotBoxFocusIndex(CurrentSaveSlotBoxFocusIndex - 1);
}

void UNewSaveSlotBoxUI::StartGame() const
{
	// SlotIndex는 위젯에서 설정
	if (CurrentSaveSlotBoxFocusIndex != -1)
	{
		auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
		check(TitleController);
		TitleController->FadeOut();

		int32 SlotIndexToUseTimer = CurrentSaveSlotBoxFocusIndex;
		
		FTimerHandle FadeTimerHandle;
		TWeakObjectPtr WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(
			FadeTimerHandle,
			[WeakThis, SlotIndexToUseTimer]()
			{
				if (WeakThis.IsValid())
				{
					check(WeakThis->GetSaveManagerSubsystemInstance());
					WeakThis->GetSaveManagerSubsystemInstance()->StartNewGame(SlotIndexToUseTimer);	
				}
			},
			TitleController->GetFadeDuration() + 0.1f,
			false
			);
	}
}

void UNewSaveSlotBoxUI::LoadGame() const
{
	if (CurrentSaveSlotBoxFocusIndex != -1)
	{
		auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
		check(TitleController);
		TitleController->FadeOut();

		int32 SlotIndexToUseTimer = CurrentSaveSlotBoxFocusIndex;

		FTimerHandle FadeTimerHandle;
		TWeakObjectPtr WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(
			FadeTimerHandle,
			[WeakThis, SlotIndexToUseTimer]()
			{
				if (WeakThis.IsValid())
				{
					check(WeakThis->GetSaveManagerSubsystemInstance());
					WeakThis->GetSaveManagerSubsystemInstance()->LoadSaveDataAndOpenLevel(SlotIndexToUseTimer);	
				}
			},
			TitleController->GetFadeDuration() + 0.1f,
			false
			);
	}
}

void UNewSaveSlotBoxUI::DeleteGame()
{
	if (CurrentSaveSlotBoxFocusIndex != -1)
	{
		SaveManagerSubsystemInstance->DeleteSaveSlot(CurrentSaveSlotBoxFocusIndex);
		RefreshSlotData(ESlateVisibility::Visible);
	}
}

void UNewSaveSlotBoxUI::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* GameInstance = GetWorld()->GetGameInstance();
	auto* SaveManagerSubsystem = GameInstance->GetSubsystem<USaveManagerSubsystem>();
	
	ensureMsgf(SaveManagerSubsystem, TEXT("Casting failed to SaveManagerSubsystem in UNewSettingWidget"));
	
	if (SaveManagerSubsystem)
	{
		SaveManagerSubsystemInstance = SaveManagerSubsystem;
	}

	const TArray<UWidget*> SlotArray = SaveSlotBox->GetAllChildren();
	if (SlotArray.Num() > 0)
	{
		for (int32 i = 0; i < SlotArray.Num(); i++)
		{
			auto* SaveSlot = Cast<UNewSaveSlot>(SlotArray[i]);
			if (SaveSlot)
			{
				SaveSlot->SetSaveSlotIndex(i);
				SaveSlots.AddUnique(SaveSlot);
			}
		}
	}
	MaxSaveSlotBoxFocusIndex = SaveSlots.Num()-1;
	
	BindInputActionDelegates();

	OnVisibilityChanged.AddDynamic(this, &UNewSaveSlotBoxUI::RefreshSlotData);
}

void UNewSaveSlotBoxUI::BindInputActionDelegates()
{
	auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
	if (TitleController)
	{
		TitleController->OnPressedStartGame.BindUObject(this, &UNewSaveSlotBoxUI::OnStartGame);
		TitleController->OnPressedDeleteGame.BindUObject(this, &UNewSaveSlotBoxUI::OnDeleteGame);
		TitleController->OnPressedBackToMainMenu.BindUObject(this, &UNewSaveSlotBoxUI::OnBackToTitleMenu);
	}
}
