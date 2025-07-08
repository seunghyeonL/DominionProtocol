// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewTitleMenuWidget.generated.h"

class UTitleMenuButton;

UCLASS()
class DOMINIONPROTOCOL_API UNewTitleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void ChangeButtonBoxFocusIndex(const int32 NewFocusIndex);

	UFUNCTION(BlueprintCallable)
	void IncreaseButtonBoxFocusIndex();

	UFUNCTION(BlueprintCallable)
	void DecreaseButtonBoxFocusIndex();
	

protected:
	UPROPERTY()
	TArray<TObjectPtr<UTitleMenuButton>> TitleMenuButtons;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ButtonBox;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentButtonBoxFocusIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxButtonBoxFocusIndex = 0;
};
