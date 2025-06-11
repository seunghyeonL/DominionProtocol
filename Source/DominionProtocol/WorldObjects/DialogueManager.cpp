#include "DialogueManager.h"
#include "WorldObjects/Helper.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"
#include "EnumAndStruct/FDialogueData.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"

void UDialogueManager::LoadDialogueDataTable()
{
	if (DialogueDataTable) return;

	static const FString Path = TEXT("/Game/Data/DT_DialogueData.DT_DialogueData");
	DialogueDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));

	if (DialogueDataTable)
	{
		Debug::Print(TEXT("O DialogueDataTable 로딩 성공"));
	}
	else
	{
		Debug::Print(TEXT("X DialogueDataTable 로딩 실패"));
	}
}

bool UDialogueManager::TryStartDialogueIfExists(EGameStoryState InState)
{
	LoadDialogueDataTable();

	Debug::Print(TEXT("UDialogueSubsystem::TryStartDialogueIfExists()"));
	if (!DialogueDataTable) return false;

	TArray<FDialogueData*> AllRows;
	DialogueDataTable->GetAllRows(TEXT("Dialogue"), AllRows);

	CurrentDialogueLines.Empty();

	for (FDialogueData* Row : AllRows)
	{
		if (Row)
		{
			FString RowState = UEnum::GetValueAsString(Row->StoryState);
			FString CurrentState = UEnum::GetValueAsString(InState);

			if (Row->StoryState == InState)
			{
				CurrentDialogueLines.Add(Row);
			}
		}
	}

	if (CurrentDialogueLines.Num() == 0)
	{
		Debug::Print(TEXT("UDialogueSubsystem: NoLine, Return False"));
		return false;
	}
		
	CurrentStoryState = InState;
	CurrentLineIndex = 0;
	ExecuteDialogueLine();
	Debug::Print(TEXT("UDialogueSubsystem: Return true"));
	return true;
}

void UDialogueManager::AdvanceDialogue()
{
	++CurrentLineIndex;
	if (CurrentLineIndex < CurrentDialogueLines.Num())
	{
		ExecuteDialogueLine();
	}
	else
	{
		Debug::Print("대사 종료");
	}
}

void UDialogueManager::ExecuteDialogueLine()
{
	Debug::Print(TEXT("UDialogueSubsystem::ExecuteDialogueLine()"));
	if (!CurrentDialogueLines.IsValidIndex(CurrentLineIndex)) return;
	const FDialogueData* Line = CurrentDialogueLines[CurrentLineIndex];

	switch (Line->EventType)
	{
	case EDialogueEventType::TextOnly:
		Debug::Print(TEXT("====================="));
		if (!Line->DialogueText.IsEmpty())
		{
			Debug::Print(Line->DialogueText.ToString());
			UE_LOG(LogTemp, Display, TEXT("%s"), *Line->DialogueText.ToString());
		}
		Debug::Print(TEXT("====================="));
		AdvanceDialogue();
		break;
	case EDialogueEventType::SpawnHelper:
		AdvanceDialogue();
		//TriggerHelperAppear();
		return;
	case EDialogueEventType::DespawnHelper:
		AdvanceDialogue();
		//TriggerHelperDisappear();
		return;
	default:
		break;
	}
}

void UDialogueManager::TriggerHelperAppear()
{
	if (!HelperClass) return;

	FVector SpawnLocation = FVector::ZeroVector; // Crack에서 위치 받아서 설정
	CurrentHelper = GetWorld()->SpawnActor<AHelper>(HelperClass, SpawnLocation, FRotator::ZeroRotator);
	if (CurrentHelper)
	{
		CurrentHelper->Appear(SpawnLocation);
		OnHelperAppearFinished();
	}
}

void UDialogueManager::TriggerHelperDisappear()
{
	if (CurrentHelper)
	{
		CurrentHelper->Disappear();
		OnHelperDisappearFinished();
	}
}

void UDialogueManager::OnHelperAppearFinished()
{
	AdvanceDialogue();
}

void UDialogueManager::OnHelperDisappearFinished()
{
	AdvanceDialogue();
}
