// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/TitleMenuButton.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UTitleMenuButton::SetButtonInfo()
{
	Sizer->SetWidthOverride(Size.X);
	Sizer->SetHeightOverride(Size.Y);
	ButtonName->SetText(ButtonNameText);
}

void UTitleMenuButton::GetFocus()
{
	GetFocusEffect();
}

void UTitleMenuButton::LoseFocus()
{
	LoseFocusEffect();
}
