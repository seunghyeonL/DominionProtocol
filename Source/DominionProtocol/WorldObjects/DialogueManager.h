#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "EnumAndStruct/FDialogueData.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
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
	UDataTable* LoadDialogueDataTable(const FString& Path);
	bool TryStartDialogueIfExists(EGameStoryState InState, const FVector& CrackLocation, const FRotator& CrackRotation);
	bool TryStartDialogueByID(const FString& DialogueID);
	
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
	FString CurrentDialogueID;

	FVector CachedHelperSpawnLocation;
	FRotator CachedHelperSpawnRotation;
};
