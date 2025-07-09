// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewTitleHUDWidget.h"

#include "Player/TitleController.h"

void UNewTitleHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BindTopUIChangeDelegate();
	
	ensureMsgf(TitleMenuUI, TEXT("TitleMenuUI Not Connected"));
	ensureMsgf(SaveSlotBoxUI, TEXT("SaveSlotBoxUI Not Connected"));
	
	auto* PlayerController = Cast<ATitleController>(GetOwningPlayer());
	if (PlayerController)
	{
		TitleController = PlayerController;
	}

	PushUI(TitleMenuUI);
	TitleController->SetupMappingContext(TitleController->TitleMenuUIMappingContext);
}

void UNewTitleHUDWidget::ActivateSaveSlotBoxUI()
{
	PushUI(SaveSlotBoxUI);
}

void UNewTitleHUDWidget::DeactivateTitleMenuUI()
{
	PopSpecificUI(SaveSlotBoxUI);
}

void UNewTitleHUDWidget::ChangeMappingContext(UUserWidget* NewTopUI)
{
	if (NewTopUI == SaveSlotBoxUI)
	{
		TitleController->SetupMappingContext(TitleController->TitleSlotUIMappingContext);
	}
	else if (NewTopUI == TitleMenuUI)
	{
		TitleController->SetupMappingContext(TitleController->TitleMenuUIMappingContext);
	}
	else
	{
		if (NewTopUI)
		{
			UE_LOG(LogTemp, Warning, TEXT("NewTopUI is %s"), *NewTopUI->GetName());	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NewTopUI is NULL"));	
		}
	}
}

void UNewTitleHUDWidget::BindTopUIChangeDelegate()
{
	OnCurrentTopUIChanged.AddUObject(this, &UNewTitleHUDWidget::ChangeMappingContext);
}