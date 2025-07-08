// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewSaveSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"
#include "EnumAndStruct/FCrackImageData.h"

void UNewSaveSlot::SetSaveSlotInfo()
{
	const TArray<FSaveSlotMetaData> SaveSlotArray = SaveManagerSubsystemInstance->GetSaveSlotArray();
	if (!SaveSlotArray.IsValidIndex(SaveSlotIndex))
	{
		return;
	}
	
	const FSaveSlotMetaData SaveSlotMetaData = SaveSlotArray[SaveSlotIndex];
	ExistSaveSlotData = SaveSlotMetaData.SaveSlotExist;
	if (ExistSaveSlotData)
	{
		if (PastCrackImageDataTable && PresentCrackImageDataTable)
		{
			const FString SaveSlotCrackName = SaveSlotMetaData.RecentCrackName.ToString();

#pragma region SearchDataTable 
			auto SearchDataTable = [&](const UDataTable* DataTableToSearch, const FString& DebugTableName) -> bool
			{
				if (!DataTableToSearch)
				{
					return false;
				}

				const TArray<FName> RowNames = DataTableToSearch->GetRowNames();
				for (const FName& RowName : RowNames)
				{
					FCrackImageData* FoundRow = DataTableToSearch->FindRow<FCrackImageData>(RowName, TEXT("SearchSingleDataTable"));
					if (FoundRow)
					{
						if (FoundRow->CrackDescription.ToString().Contains(SaveSlotCrackName))
						{
							CrackMapImage->SetBrushFromTexture(FoundRow->CrackImageData);
							CurrentCrackName->SetText(SaveSlotMetaData.RecentCrackName);
							return true;
						}
					}
				}
				return false;
			};
#pragma endregion

			if (SearchDataTable(PastCrackImageDataTable, TEXT("PastCrackImageDataTable")))
			{
			}
			else if (SearchDataTable(PresentCrackImageDataTable, TEXT("PresentCrackImageDataTable")))
			{
			}
		}

		const FString GameIndexString = FString::Printf(TEXT("게임 번호 %d"), SaveSlotMetaData.SaveSlotIndex+1);
		GameIndex->SetText(FText::FromString(GameIndexString));

		const FDateTime SaveDataTime = SaveSlotMetaData.SaveDateTime;
		const FString SaveTimeString = FString::Printf(TEXT("마지막 저장 일 : %d / %d / %d"), SaveDataTime.GetYear(), SaveDataTime.GetMonth(), SaveDataTime.GetDay());
		SaveTime->SetText(FText::FromString(SaveTimeString));
		
		const FString PlayTimeString = FString::Printf(TEXT("총 플레이 시간 : %d : %d"), SaveSlotMetaData.PlayTime/60, SaveSlotMetaData.PlayTime%60);
		PlayTime->SetText(FText::FromString(PlayTimeString));

		CurrentLevelName->SetText(SaveSlotMetaData.PlayingLevelDisplayName);

		const FString PlayerLevelString = FString::Printf(TEXT("플에이어 레벨 : %d"), SaveSlotMetaData.PlayerLevel);
		PlayerLevel->SetText(FText::FromString(PlayerLevelString));
	}
	else
	{
		SetSaveSlotEmpty();
	}
	
}

void UNewSaveSlot::SetSaveSlotEmpty()
{
	ExistSaveSlotData = false;
	CrackMapImage->SetBrushFromTexture(nullptr);
	GameIndex->SetText(FText::FromString(TEXT("데이터없음")));
	SaveTime->SetText(FText::FromString(TEXT("-- / -- / --")));
	PlayTime->SetText(FText::FromString(TEXT("총 플레이 시간 -- : --")));
	CurrentLevelName->SetText(FText::FromString(TEXT("-")));
	CurrentCrackName->SetText(FText::FromString(TEXT("-")));
	PlayerLevel->SetText(FText::FromString(TEXT("")));
}

void UNewSaveSlot::GetFocus()
{
	GetFocusEffect();
}

void UNewSaveSlot::LoseFocus()
{
	LoseFocusEffect();
}

void UNewSaveSlot::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* GameInstance = GetWorld()->GetGameInstance();
	auto* SaveManagerSubsystem = GameInstance->GetSubsystem<USaveManagerSubsystem>();
	
	ensureMsgf(SaveManagerSubsystem, TEXT("Casting failed to SaveManagerSubsystem in UNewSettingWidget"));
	
	if (SaveManagerSubsystem)
	{
		SaveManagerSubsystemInstance = SaveManagerSubsystem;
	}
}
