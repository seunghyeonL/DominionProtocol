// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/NewCrackMenuWidget.h"

#include "Player/InGameController.h"

void UNewCrackMenuWidget::OnPressedBackButton()
{
	PressedBackButton();
}

void UNewCrackMenuWidget::OnPressedConfirmButton()
{
	PressedConfirmButton();
}

void UNewCrackMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInputActionDelegates();
}

void UNewCrackMenuWidget::BindInputActionDelegates()
{
	auto* InGameController = Cast<AInGameController>(GetOwningPlayer());
	if (InGameController)
	{
		InGameController->OnPressedCrackMenuBackButtonEvent.BindUObject(this, &UNewCrackMenuWidget::OnPressedBackButton);
		InGameController->OnPressedCrackMenuConfirmButtonEvent.BindUObject(this, &UNewCrackMenuWidget::OnPressedConfirmButton);
	}
}
