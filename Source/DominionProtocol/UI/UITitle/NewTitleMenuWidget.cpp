// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewTitleMenuWidget.h"

void UNewTitleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensureMsgf(SaveSlotBox, TEXT("SaveSlotBox Not Connected"));
}
