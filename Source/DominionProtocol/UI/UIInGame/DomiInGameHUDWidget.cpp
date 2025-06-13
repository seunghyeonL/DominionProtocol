// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInGameHUDWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Player/InGameController.h"


void UDomiInGameHUDWidget::OnPlayerDeath()
{
	ShowDeathScriptWidget();
}

void UDomiInGameHUDWidget::OnPlayerSpawn()
{
	HideDeathScriptWidget();
}

void UDomiInGameHUDWidget::OnSwitchShowAndHideOnInGameMenuWidget()
{
	SwitchShowAndHideOnInGameMenuWidget();
}

void UDomiInGameHUDWidget::OnSwitchShowAndHideCrackWarpWidget()
{
	SwitchShowAndHideCrackWarpWidget();
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
	ensure(CrackWrapWidget);

	AActor* OwningActor = GetOwningPlayerPawn();
	if (OwningActor)
	{
		SetupStatusBarWidget(OwningActor);
	}

	auto* InGameController = Cast<AInGameController>(GetOwningPlayer());
	if (InGameController)
	{
		OwningController = InGameController;
	}
}

void UDomiInGameHUDWidget::SetupStatusBarWidget(const AActor* OwningActor)
{
	ensure(IsValid(OwningActor));
	
	auto* StatusComp = OwningActor->GetComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		StatusComp->OnDeath.AddUObject(this, &UDomiInGameHUDWidget::OnPlayerDeath);
	}

	auto* GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnPlayerSpawn.AddUObject(this, &UDomiInGameHUDWidget::OnPlayerSpawn);
	}
}




