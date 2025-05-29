// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewMainMenuWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UNewMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UFUNCTION(BlueprintCallable)
	void ChangeWidgetZOrder(const UUserWidget* TargetWidget, const int32 NewZOrder);

	UFUNCTION()
	void EventPressButtonZ();

	UFUNCTION()
	void EventPressButtonC();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonZ();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonC();
};
