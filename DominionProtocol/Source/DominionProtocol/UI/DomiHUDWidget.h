// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiHUDWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiHUDWidget : public UUserWidget
{
	GENERATED_BODY()


public:


protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDomiStatusBarWidget> StatusBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDomiAbilitySlotsWidget> AbilitySlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDomiItemSlotsWidget> ItemSlots;
	
};
