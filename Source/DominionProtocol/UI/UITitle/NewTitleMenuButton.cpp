// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/NewTitleMenuButton.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UNewTitleMenuButton::SetButtonInfo()
{
	Sizer->SetWidthOverride(Size.X);
	Sizer->SetHeightOverride(Size.Y);
	ButtonName->SetText(ButtonNameText);
}

void UNewTitleMenuButton::GetFocus()
{
	GetFocusEffect();
}

void UNewTitleMenuButton::LoseFocus()
{
	LoseFocusEffect();
}
