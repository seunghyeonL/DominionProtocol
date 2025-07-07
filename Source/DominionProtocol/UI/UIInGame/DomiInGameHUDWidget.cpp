// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInGameHUDWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Player/InGameController.h"


void UDomiInGameHUDWidget::OnShowCrackMenuWidget()
{
	ShowCrackMenuWidget();

	OwningController->SetupMappingContext(OwningController->CrackMenuMappingContext);
}

void UDomiInGameHUDWidget::OnMainMenuSwitchShowAndHideWidget()
{
	MainMenuSwitchShowAndHideWidget();
}

void UDomiInGameHUDWidget::OnDialogueChangedNextStoryState()
{
	DialogueChangedNextStoryState();
}

void UDomiInGameHUDWidget::ChangeWidgetZOrder(const UUserWidget* TargetWidget, const int32 NewZOrder)
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

void UDomiInGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(StatusBarWidget);
	ensure(ItemSlotsWidget);
	ensure(InteractionWidget);
	ensure(CrackMenuWidget);

	auto* InGameController = Cast<AInGameController>(GetOwningPlayer());
	if (InGameController)
	{
		OwningController = InGameController;
	}

	BindInputActionDelegates();
}

void UDomiInGameHUDWidget::BindInputActionDelegates()
{
	auto* Controller = Cast<AInGameController>(GetOwningPlayer());
	if (Controller)
	{
		Controller->OnPressedMainMenuSwitchShowAndHideWidgetEvent.BindUObject(this, &UDomiInGameHUDWidget::OnMainMenuSwitchShowAndHideWidget);
		Controller->OnPressedDialogueChangedNextStoryState.BindUObject(this, &UDomiInGameHUDWidget::OnDialogueChangedNextStoryState);
	}
}




