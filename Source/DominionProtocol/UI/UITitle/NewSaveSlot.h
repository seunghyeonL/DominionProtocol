// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSaveSlot.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class DOMINIONPROTOCOL_API UNewSaveSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSaveSlotIndex(const int32 NewSaveSlotIndex) { SaveSlotIndex = NewSaveSlotIndex; }
	
	void SetSaveSlotInfo();

	UFUNCTION(BlueprintCallable)
	void SetSaveSlotEmpty();

	void GetFocus();

	UFUNCTION(BlueprintImplementableEvent)
	void GetFocusEffect();

	UFUNCTION(BlueprintCallable)
	void LoseFocus();

	UFUNCTION(BlueprintImplementableEvent)
	void LoseFocusEffect();

	int32 GetSaveSlotIndex() const { return SaveSlotIndex; }

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> PastCrackImageDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> PresentCrackImageDataTable;
	
	UPROPERTY()
	TObjectPtr<class USaveManagerSubsystem> SaveManagerSubsystemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SaveSlotIndex = -1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CrackMapImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GameIndex;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SaveTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentLevelName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentCrackName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerLevel;

	UPROPERTY(BlueprintReadOnly)
	bool ExistSaveSlotData = false;
	
};
