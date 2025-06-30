#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "FDialogueData.generated.h"

UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
	TextOnly,
	SpawnHelper,
	DespawnHelper
};

USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

	FDialogueData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameStoryState StoryState = EGameStoryState::Tutorial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LineIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogueText;
};