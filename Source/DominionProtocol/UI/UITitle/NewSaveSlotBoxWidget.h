// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSaveSlotBoxWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UNewSaveSlotBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnPressStartGame();

	UFUNCTION()
	void OnPressDeleteGame();

	UFUNCTION()
	void OnPressBackToMainMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void PressStartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void PressDeleteGame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PressBackToMainMenu();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BindInputActionDelegates();
	
	
};
