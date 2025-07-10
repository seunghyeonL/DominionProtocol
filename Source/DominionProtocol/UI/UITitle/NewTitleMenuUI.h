// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewTitleMenuUI.generated.h"

class UNewTitleMenuButton;

UCLASS()
class DOMINIONPROTOCOL_API UNewTitleMenuUI : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	UFUNCTION(BlueprintCallable)
	void ChangeButtonBoxFocusIndex(const int32 NewFocusIndex);

	UFUNCTION()
	void IncreaseButtonBoxFocusIndex();

	UFUNCTION()
	void DecreaseButtonBoxFocusIndex();
	
	UFUNCTION()
	void OnMoveSelectionUp();

	UFUNCTION()
	void OnMoveSelectionDown();

	UFUNCTION()
	void OnConfirmSelection();

	virtual void NativeConstruct() override;

	void BindInputActionDelegates();
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<UNewTitleMenuButton>> TitleMenuButtons;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ButtonBox;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentButtonBoxFocusIndex = -1;

	UPROPERTY()
	int32 MaxButtonBoxFocusIndex = 0;
};
