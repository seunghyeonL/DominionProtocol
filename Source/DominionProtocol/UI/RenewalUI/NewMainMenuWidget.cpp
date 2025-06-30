// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewMainMenuWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Player/InGameController.h"

void UNewMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInputActionDelegates();
}

void UNewMainMenuWidget::BindInputActionDelegates()
{
	auto* InGameController = Cast<AInGameController>(GetOwningPlayer());
	if (InGameController)
	{
		InGameController->OnPressedMainMenuButtonQEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonQ);
		InGameController->OnPressedMainMenuButtonEEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonE);
		InGameController->OnPressedMainMenuButtonREvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonR);
		InGameController->OnPressedMainMenuButtonAEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonA);
		InGameController->OnPressedMainMenuButtonDEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonD);
		InGameController->OnPressedMainMenuButtonZEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonZ);
		InGameController->OnPressedMainMenuButtonCEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonC);
		InGameController->OnPressedMainMenuButtonSpaceBarEvent.BindUObject(this, &UNewMainMenuWidget::OnPressButtonSpaceBar);
	}
}

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

void UNewMainMenuWidget::OnPressButtonZ()
{
	PressButtonZ();
}

void UNewMainMenuWidget::OnPressButtonC()
{
	PressButtonC();
}

void UNewMainMenuWidget::OnPressButtonA()
{
	PressButtonA();
}

void UNewMainMenuWidget::OnPressButtonD()
{
	PressButtonD();
}

void UNewMainMenuWidget::OnPressButtonE()
{
	PressButtonE();
}

void UNewMainMenuWidget::OnPressButtonR()
{
	PressButtonR();
}

void UNewMainMenuWidget::OnPressButtonSpaceBar()
{
	PressButtonSpaceBar();
}

void UNewMainMenuWidget::OnPressButtonQ()
{
	PressButtonQ();
}
