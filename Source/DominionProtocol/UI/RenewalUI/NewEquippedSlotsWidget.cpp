// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewEquippedSlotsWidget.h"

#include "Components/ItemComponent/ItemComponent.h"


void UNewEquippedSlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindItemSlotsDelegate();
}

void UNewEquippedSlotsWidget::BindItemSlotsDelegate()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		ItemComponent = PlayerCharacter->GetComponentByClass<UItemComponent>();
		if (ItemComponent)
		{
			ItemComponent->OnInventoryEquippedSlotItemsChanged.BindUObject(this, &UNewEquippedSlotsWidget::OnUpdateEquippableSlotItems);
			ItemComponent->OnInventoryConsumableSlotItemsChanged.BindUObject(this, &UNewEquippedSlotsWidget::OnUpdateConsumableSlotItems);
		}
	}
}

void UNewEquippedSlotsWidget::OnUpdateEquippableSlotItems()
{
	EquippableSlotMap = ItemComponent->GetEquippedDisplayItems();

	UpdateEquippableSlotItems();
}

void UNewEquippedSlotsWidget::OnUpdateConsumableSlotItems()
{
	ConsumableSlotMap = ItemComponent->GetConsumableDisplayItems();

	UpdateConsumableSlotItems();
}