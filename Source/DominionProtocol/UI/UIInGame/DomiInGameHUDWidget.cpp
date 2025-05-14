// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInGameHUDWidget.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"

void UDomiInGameHUDWidget::OnPlayerDeath()
{
	ShowDeathScriptWidget();
}

void UDomiInGameHUDWidget::OnPlayerSpawn()
{
	HideDeathScriptWidget();
}

void UDomiInGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(StatusBarWidget);
	ensure(AbilitySlotsWidget);
	ensure(ItemSlotsWidget);
	ensure(InGameMenuWidget);

	AActor* OwningActor = GetOwningPlayerPawn();
	if (OwningActor)
	{
		SetupStatusBarWidget(OwningActor);	
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
