// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewDialogueWidget.generated.h"

enum class EGameStoryState : uint8;
class UDialogueManager;

UCLASS()
class DOMINIONPROTOCOL_API UNewDialogueWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void UpdateDialogueWidget(const FText NewText);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateDialogueWidget();
	

protected:
	virtual void NativeConstruct() override;

	void BindCreateDialogueDelegate();
	void BindDialogueDelegate(UDialogueManager* DialogueManager);

protected:
	UPROPERTY(BlueprintReadOnly)
	FText CurrentDialogueText;
};
