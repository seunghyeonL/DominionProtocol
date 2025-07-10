// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSaveSlotBoxUI.generated.h"

class UVerticalBox;
class UNewSaveSlot;

UCLASS()
class DOMINIONPROTOCOL_API UNewSaveSlotBoxUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RefreshSlotData(const ESlateVisibility VisibilityState);
	
	// For Input Action 
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeleteGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBackToTitleMenu();

	UFUNCTION()
	void OnMoveSelectionUp();

	UFUNCTION()
	void OnMoveSelectionDown();

	FORCEINLINE class USaveManagerSubsystem* GetSaveManagerSubsystemInstance() const { return SaveManagerSubsystemInstance; }

protected:
	UFUNCTION(BlueprintCallable)
	void ChangeSaveSlotBoxFocusIndex(const int32 NewFocusIndex);

	UFUNCTION(BlueprintCallable)
	void IncreaseSaveSlotBoxFocusIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseSaveSlotBoxFocusIndex();
	
	UFUNCTION(BlueprintCallable)
	void StartGame() const;
	
	UFUNCTION(BlueprintCallable)
	void LoadGame() const;
	
	UFUNCTION(BlueprintCallable)
	void DeleteGame();

	virtual void NativeConstruct() override;

	UFUNCTION()
	void BindInputActionDelegates();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USaveManagerSubsystem> SaveManagerSubsystemInstance;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> SaveSlotBox;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UNewSaveSlot>> SaveSlots;
	
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentSaveSlotBoxFocusIndex = -1;

	UPROPERTY()
	int32 MaxSaveSlotBoxFocusIndex = 0;
	
};
