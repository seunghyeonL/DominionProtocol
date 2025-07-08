// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseHUDWidget.generated.h"

UCLASS(Abstract)
class DOMINIONPROTOCOL_API UBaseHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UUserWidget* PushUI(UUserWidget* WidgetToPush);

	UFUNCTION()
	void PopUI();

	UFUNCTION()
	void PopSpecificUI(UUserWidget* WidgetToPop);

	UFUNCTION()
	void UpdateAllWidgetZOrderInStack();

	UFUNCTION()
	void ClearUIStack();

	UFUNCTION()
	virtual void SetInputModeForUIStack();

	
protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> RootCanvasPanel;

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> ActivatedUIStack;
	
};
