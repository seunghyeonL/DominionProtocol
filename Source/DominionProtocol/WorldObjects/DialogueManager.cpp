#include "DialogueManager.h"

#include "DyingHelper.h"
#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "DomiFramework/WorldActorManage/ActorStateComponent.h"
#include "WorldObjects/Helper.h"
#include "WorldObjects/DyingHelper.h"
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

	const FString Path = TEXT("/Game/Data/DT_HelperDialogueData.DT_HelperDialogueData");
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

bool UDialogueManager::TryStartDialogueByID(const FString& DialogueID, AActor* TalkActor)
{
	const FString Path = TEXT("/Game/Data/DT_DialogueData.DT_DialogueData");
	UDataTable* Table = LoadDialogueDataTable(Path);
	if (!Table) return false;

	// (이미 죽은 경우에 대한 처리 위함)
	if (IsValid(TalkActor))
	{
		TalkActorCache = TalkActor;
	}
	
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

	// 다음 문장 있을 때
	if (CurrentLineIndex < CurrentDialogueLines.Num())
	{
		ExecuteDialogueLine();
	}
	// 다음 문장 없을 때
	else
	{
		// 균열 등장 조력자인 경우
		if (IsValid(CurrentHelper))
		{
			TriggerHelperDisappear();
		}
		// 죽어가는 조력자인 경우(보스3 전투공간)
		else if (TalkActorCache->IsA(ADyingHelper::StaticClass()))
		{
			ADyingHelper* DyingHelper = Cast<ADyingHelper>(TalkActorCache);
			if (IsValid(DyingHelper))
			{
				DyingHelper->Die();
			}
		}
		// 죽어가는 마녀일 경우
		else if (TalkActorCache->IsA(ABoss4Enemy::StaticClass()))
		{
			//로직 작성 필요
			// ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(TalkActorCache);
			// if (IsValid(Boss4))
			// {
			// 	Boss4->Die();
			// }
		}
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
		FVector TargetOffset = FVector(0.f, 0.f, 100.f);
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		FTransform PlayerTransform(Player->GetActorRotation(), Player->GetActorLocation());
		CurrentHelper->SetViewTargetLocAndRot(PlayerTransform.TransformPosition(TargetOffset), CachedHelperSpawnRotation);
		CurrentHelper->SetViewToHelper();
	}

	ExecuteDialogueLine();
}

void UDialogueManager::TriggerHelperDisappear()
{
	if (CurrentHelper)
	{
		Debug::Print(TEXT("UDialogueManager::TriggerHelperDisappear()"));
		CurrentHelper->SetViewToPlayer();
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
