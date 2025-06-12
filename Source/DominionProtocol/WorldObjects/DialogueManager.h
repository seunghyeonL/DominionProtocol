#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "EnumAndStruct/FDialogueData.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DialogueManager.generated.h"

class AHelper;
class UDataTable;

UCLASS()
class DOMINIONPROTOCOL_API UDialogueManager : public UObject
{
	GENERATED_BODY()
	
public:

	void LoadDialogueDataTable();
	bool TryStartDialogueIfExists(EGameStoryState InState, FVector CrackLocation);
	void AdvanceDialogue();

private:
	void ExecuteDialogueLine();
	void TriggerHelperAppear();
	void TriggerHelperDisappear();
	void OnHelperAppearFinished();
	void OnHelperDisappearFinished();

public:
	UPROPERTY()
	TSubclassOf<AHelper> HelperClass;
private:
	UPROPERTY(EditAnywhere)
	UDataTable* DialogueDataTable;

	UPROPERTY()
	AHelper* CurrentHelper;

	TArray<FDialogueData*> CurrentDialogueLines;

	int32 CurrentLineIndex = 0;
	EGameStoryState CurrentStoryState;

	FVector CachedHelperSpawnLocation;
};
