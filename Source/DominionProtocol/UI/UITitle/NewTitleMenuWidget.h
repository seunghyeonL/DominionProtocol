// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewTitleMenuWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UNewTitleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UNewSaveSlotBoxWidget> SaveSlotBox;
};
