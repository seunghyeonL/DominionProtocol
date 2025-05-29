// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiItemSlotsWidget.h"

#include "Components/ItemComponent/ItemComponent.h"

void UDomiItemSlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindItemSlotsDelegate();
}

void UDomiItemSlotsWidget::BindItemSlotsDelegate()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		ItemComponent = PlayerCharacter->GetComponentByClass<UItemComponent>();
		if (ItemComponent)
		{
			ItemComponent->OnInventoryEquippedSlotItemsChanged.BindUObject(this, &UDomiItemSlotsWidget::OnUpdateEquippableSlotItems);
			ItemComponent->OnInventoryConsumableSlotItemsChanged.BindUObject(this, &UDomiItemSlotsWidget::OnUpdateConsumableSlotItems);
		}
	}
}

void UDomiItemSlotsWidget::OnUpdateEquippableSlotItems()
{
	InventoryEquippableSlotItems = ItemComponent->GetEquippedDisplayItems();

	UpdateEquippableSlotItems();
}

void UDomiItemSlotsWidget::OnUpdateConsumableSlotItems()
{
	InventoryConsumableSlotItems = ItemComponent->GetConsumableDisplayItems();

	UpdateConsumableSlotItems();
}
