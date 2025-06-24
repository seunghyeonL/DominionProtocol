#include "DialogueManager.h"
#include "WorldObjects/Helper.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"
#include "EnumAndStruct/FDialogueData.h"
#include "GameFramework/CharacterMovementComponent.h"

UDataTable* UDialogueManager::LoadDialogueDataTable(const FString& Path)
{
	UDataTable* Table = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));
	if (!Table)
	{
		Debug::Print(FString::Printf(TEXT("X 로딩 실패: %s"), *Path));
	}
	else
	{
		Debug::Print(FString::Printf(TEXT("O 로딩 성공: %s"), *Path));
	}

	return Table;
}

bool UDialogueManager::TryStartDialogueIfExists(EGameStoryState InState, const FVector& CrackLocation, const FRotator& CrackRotation)
{
	CachedHelperSpawnLocation = CrackLocation;
	CachedHelperSpawnRotation = CrackRotation;
	CachedHelperSpawnRotation.Yaw -= 40.f;

	const FString Path = TEXT("/Game/Data/DT_HelperDialogueData");
	UDataTable* Table = LoadDialogueDataTable(Path);
	if (!Table) return false;

	TArray<FDialogueData*> AllRows;
	DialogueDataTable = Table;
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
		Debug::Print(TEXT("UDialogueSubsystem: 대사 없음"));
		return false;
	}
		
	CurrentStoryState = InState;
	CurrentLineIndex = 0;
	TriggerHelperAppear();
	return true;
}

bool UDialogueManager::TryStartDialogueByID(const FString& DialogueID)
{
	const FString Path = TEXT("/Game/Data/DT_DialogueData");
	UDataTable* Table = LoadDialogueDataTable(Path);
	if (!Table) return false;

	TArray<FDialogueData*> AllRows;
	DialogueDataTable = Table;
	DialogueDataTable->GetAllRows(TEXT("Dialogue"), AllRows);

	CurrentDialogueLines.Empty();

	for (FDialogueData* Row : AllRows)
	{
		if (Row && Row->DialogueID.ToString() == DialogueID)
		{
			CurrentDialogueLines.Add(Row);
		}
	}

	if (CurrentDialogueLines.Num() == 0)
	{
		Debug::Print(TEXT("UDialogueManager::대사 없음"));
		return false;
	}

	CurrentDialogueID = DialogueID;
	CurrentLineIndex = 0;
	ExecuteDialogueLine();
	return true;
}

// 다음으로 넘어가는 함수
void UDialogueManager::AdvanceDialogue()
{
	++CurrentLineIndex;
	if (CurrentLineIndex < CurrentDialogueLines.Num())
	{
		ExecuteDialogueLine();
	}
	else
	{
		TriggerHelperDisappear();
	}
}

void UDialogueManager::ExecuteDialogueLine()
{
	if (!CurrentDialogueLines.IsValidIndex(CurrentLineIndex)) return;
	const FDialogueData* Line = CurrentDialogueLines[CurrentLineIndex];

	if (!Line->DialogueText.IsEmpty())
	{
		Debug::Print(Line->DialogueText.ToString()); // 대사
		CurrentDialogueString = Line->DialogueText.ToString();
		OnUpdateDialogueText.Broadcast(FText::FromString(CurrentDialogueString)); // 대사 델리게이트
	}
}

void UDialogueManager::TriggerHelperAppear()
{
	if (!HelperClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CurrentHelper = GetWorld()->SpawnActor<AHelper>(
		HelperClass,
		CachedHelperSpawnLocation,
		CachedHelperSpawnRotation,
		SpawnParams
	);
	if (CurrentHelper)
	{
		CurrentHelper->OnAppearFinishedCallback.BindUObject(this, &UDialogueManager::OnHelperAppearFinished);

		CurrentHelper->SetDialogueManager(this);
		CurrentHelper->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		CurrentHelper->Appear(CachedHelperSpawnLocation, CachedHelperSpawnRotation);
	}

	ExecuteDialogueLine();
}

void UDialogueManager::TriggerHelperDisappear()
{
	if (CurrentHelper)
	{
		Debug::Print(TEXT("UDialogueManager::TriggerHelperDisappear()"));
		CurrentHelper->Disappear();
	}
}

void UDialogueManager::OnHelperAppearFinished()
{
	AdvanceDialogue();
}

void UDialogueManager::OnHelperDisappearFinished()
{
	Debug::Print(TEXT("UDialogueManager::OnHelperDisappearFinished()"));
	AdvanceDialogue();
}
