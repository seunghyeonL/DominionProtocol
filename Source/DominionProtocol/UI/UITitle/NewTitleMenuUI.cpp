// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewTitleMenuUI.h"

#include "Components/VerticalBox.h"
#include "Player/TitleController.h"
#include "UI/UITitle/NewTitleMenuButton.h"

void UNewTitleMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	ensureMsgf(ButtonBox, TEXT("ButtonBox Not Connected"));
	
	TArray<UWidget*> ButtonArray = ButtonBox->GetAllChildren();
	for (int32 i = 0; i < ButtonArray.Num(); i++)
	{
		auto* Button = Cast<UNewTitleMenuButton>(ButtonArray[i]);
		if (Button)
		{
			Button->SetButtonIndex(i);
			TitleMenuButtons.AddUnique(Button);
		}
	}
	MaxButtonBoxFocusIndex = TitleMenuButtons.Num() - 1;

	BindInputActionDelegates();
	ChangeButtonBoxFocusIndex(0);
}

void UNewTitleMenuUI::ChangeButtonBoxFocusIndex(const int32 NewFocusIndex)
{
	if (!TitleMenuButtons.IsEmpty())
	{
		for (int32 i = 0; i < TitleMenuButtons.Num(); i++)
		{
			if (i == NewFocusIndex)
			{
				TitleMenuButtons[i]->GetFocus();
				TitleMenuButtons[i]->SetFocus();
				CurrentButtonBoxFocusIndex = NewFocusIndex;
			}
			else
			{
				TitleMenuButtons[i]->LoseFocus();
			}
		}
	}
}

void UNewTitleMenuUI::IncreaseButtonBoxFocusIndex()
{
	if (CurrentButtonBoxFocusIndex >= MaxButtonBoxFocusIndex)
	{
		return; 
	}

	ChangeButtonBoxFocusIndex(CurrentButtonBoxFocusIndex + 1);
}

void UNewTitleMenuUI::DecreaseButtonBoxFocusIndex()
{
	if (CurrentButtonBoxFocusIndex <= 0)
	{
		return;
	}

	ChangeButtonBoxFocusIndex(CurrentButtonBoxFocusIndex - 1);
}

void UNewTitleMenuUI::OnMoveSelectionUp()
{
	DecreaseButtonBoxFocusIndex();	
}

void UNewTitleMenuUI::OnMoveSelectionDown()
{
	IncreaseButtonBoxFocusIndex();
}

void UNewTitleMenuUI::OnConfirmSelection()
{
	TitleMenuButtons[CurrentButtonBoxFocusIndex]->OnConfirm();
}

void UNewTitleMenuUI::BindInputActionDelegates()
{
	auto* TitleController = Cast<ATitleController>(GetOwningPlayer());
	if (TitleController)
	{
		TitleController->OnMenuUIMoveSelectionUp.AddUObject(this, &UNewTitleMenuUI::OnMoveSelectionUp);
		TitleController->OnMenuUIMoveSelectionDown.AddUObject(this, &UNewTitleMenuUI::OnMoveSelectionDown);
		TitleController->OnMenuUIConfirmSelection.AddUObject(this, &UNewTitleMenuUI::OnConfirmSelection);
	}
}
