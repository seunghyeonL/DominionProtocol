// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiInGameHUDWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiInGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()


public:


protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiStatusBarWidget> StatusBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiAbilitySlotsWidget> AbilitySlotsWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiItemSlotsWidget> ItemSlotsWidget;
};
