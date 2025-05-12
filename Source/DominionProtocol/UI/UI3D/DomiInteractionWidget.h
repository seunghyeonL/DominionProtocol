// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiInteractionWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBoxWidget;

	UPROPERTY()
	TArray<UUserWidget*> InteractionList; 
	
};
