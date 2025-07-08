// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewTitleMenuUI.h"

#include "Components/VerticalBox.h"
#include "UI/UITitle/TitleMenuButton.h"

void UNewTitleMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	ensureMsgf(ButtonBox, TEXT("ButtonBox Not Connected"));
	
	TArray<UWidget*> ButtonArray = ButtonBox->GetAllChildren();
	for (int32 i = 0; i < ButtonArray.Num(); i++)
	{
		auto* Button = Cast<UTitleMenuButton>(ButtonArray[i]);
		if (Button)
		{
			Button->SetButtonIndex(i);
			TitleMenuButtons.AddUnique(Button);
		}
	}
	MaxButtonBoxFocusIndex = TitleMenuButtons.Num() - 1;
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

