// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMenuButton.generated.h"

class UTextBlock;
class USizeBox;

UCLASS()
class DOMINIONPROTOCOL_API UTitleMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonInfo();

	UFUNCTION(BlueprintCallable)
	void GetFocus();
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetFocusEffect();

	UFUNCTION(BlueprintCallable)
	void LoseFocus();

	UFUNCTION(BlueprintImplementableEvent)
	void LoseFocusEffect();
	
	void SetButtonIndex(const int32 NewButtonIndex) { ButtonIndex = NewButtonIndex; }

protected:
	UPROPERTY(BlueprintReadWrite)
	int32 ButtonIndex;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> Sizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Size = {320.f,80.f};
};
