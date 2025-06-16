#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "EnumAndStruct/FDialogueData.h"
#include "DialogueManager.generated.h"

class AHelper;
class UDataTable;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateDialogueText, FText)

UCLASS()
class DOMINIONPROTOCOL_API UDialogueManager : public UObject
{
	GENERATED_BODY()
	
public:

	FOnUpdateDialogueText OnUpdateDialogueText;

	void LoadDialogueDataTable();
	bool TryStartDialogueIfExists(EGameStoryState InState, FVector CrackLocation);
	UFUNCTION(BlueprintCallable)
	void AdvanceDialogue();
	void OnHelperAppearFinished();
	void OnHelperDisappearFinished();

private:
	void ExecuteDialogueLine();
	void TriggerHelperAppear();
	void TriggerHelperDisappear();

public:
	UPROPERTY()
	TSubclassOf<AHelper> HelperClass;
private:
	UPROPERTY(EditAnywhere)
	UDataTable* DialogueDataTable;

	UPROPERTY()
	AHelper* CurrentHelper;

	TArray<FDialogueData*> CurrentDialogueLines;

	FString CurrentDialogueString;

	int32 CurrentLineIndex = 0;
	EGameStoryState CurrentStoryState;

	FVector CachedHelperSpawnLocation;
};
