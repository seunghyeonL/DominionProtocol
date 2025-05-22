// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiItemSlotsWidget.h"

#include "Components/ItemComponent/ItemComponent.h"


void UDomiItemSlotsWidget::OnSwapWeapons()
{
	UpdateDisplayEquipWeapons();
}

void UDomiItemSlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindSwapWeaponsEvent();
}

void UDomiItemSlotsWidget::BindSwapWeaponsEvent()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		auto* ItemComp = PlayerCharacter->FindComponentByClass<UItemComponent>();
		if (ItemComp)
		{
			ItemComp->OnSwapWeapons.BindUObject(this, &UDomiItemSlotsWidget::OnSwapWeapons);
		}
	}
}