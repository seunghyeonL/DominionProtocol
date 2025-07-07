// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewEquipmentWidget.h"

#include "Components/ItemComponent/ItemComponent.h"
#include "Components/StatusComponent/StatusComponent.h"

void UNewEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto* PlayerStatusComponent = GetOwningPlayerPawn()->FindComponentByClass<UStatusComponent>();
	if (PlayerStatusComponent)
	{
		StatusComponent = PlayerStatusComponent;
	}

	auto* PlayerItemComponent = GetOwningPlayerPawn()->FindComponentByClass<UItemComponent>();
	if (PlayerItemComponent)
	{
		ItemComponent = PlayerItemComponent;
		BindEquippedSlotsDelegates();
	}
}

void UNewEquipmentWidget::BindEquippedSlotsDelegates()
{
	if (ItemComponent)
	{
		ItemComponent->OnInventoryEquippedSlotItemsChanged.AddUObject(this, &UNewEquipmentWidget::OnUpdateEquippedSlots);
		ItemComponent->OnInventoryConsumableSlotItemsChanged.AddUObject(this, &UNewEquipmentWidget::OnUpdateEquippedSlots);
	}

	
}

void UNewEquipmentWidget::OnUpdateEquippedSlots()
{
	UpdateEquippedSlots();
}
