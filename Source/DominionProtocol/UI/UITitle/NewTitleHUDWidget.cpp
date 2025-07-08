// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewTitleHUDWidget.h"

void UNewTitleHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensureMsgf(TitleMenuUI, TEXT("TitleMenuUI Not Connected"));
	ensureMsgf(SaveSlotBoxUI, TEXT("SaveSlotBoxUI Not Connected"));

	PushUI(TitleMenuUI);
}

void UNewTitleHUDWidget::ActivateSaveSlotBoxUI()
{
	PushUI(SaveSlotBoxUI);
}

void UNewTitleHUDWidget::DeactivateTitleMenuUI()
{
	PopSpecificUI(SaveSlotBoxUI);
}

