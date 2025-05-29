// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewMainMenuWidget.h"

#include "Components/CanvasPanelSlot.h"

void UNewMainMenuWidget::ChangeWidgetZOrder(const UUserWidget* TargetWidget, const int32 NewZOrder)
{
	if (!IsValid(TargetWidget))
	{
		return;
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);

	if (IsValid(CanvasSlot))
	{
		CanvasSlot->SetZOrder(NewZOrder);
	}
}

void UNewMainMenuWidget::EventPressButtonZ()
{
	if (IsVisible())
	{
		PressButtonZ();
	}
}

void UNewMainMenuWidget::EventPressButtonC()
{
	if (IsVisible())
	{
		PressButtonC();	
	}
}
