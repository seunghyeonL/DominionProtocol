// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewSaveSlotBoxWidget.h"

#include "Player/TitleController.h"

void UNewSaveSlotBoxWidget::OnPressStartGame()
{
	PressStartGame();
}

void UNewSaveSlotBoxWidget::OnPressDeleteGame()
{
	PressDeleteGame();
}

void UNewSaveSlotBoxWidget::OnPressBackToMainMenu()
{
	PressBackToMainMenu();
}

void UNewSaveSlotBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInputActionDelegates();
}

void UNewSaveSlotBoxWidget::BindInputActionDelegates()
{
	auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
	if (TitleController)
	{
		TitleController->OnPressedStartGame.BindUObject(this, &UNewSaveSlotBoxWidget::OnPressStartGame);
		TitleController->OnPressedDeleteGame.BindUObject(this, &UNewSaveSlotBoxWidget::OnPressDeleteGame);
		TitleController->OnPressedBackToMainMenu.BindUObject(this, &UNewSaveSlotBoxWidget::OnPressBackToMainMenu);
	}
}
