// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/DomiInventoryWidget.h"

#include "Components/ItemComponent/ItemComponent.h"

void UDomiInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInventoryDelegates();
}

void UDomiInventoryWidget::BindInventoryDelegates()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		ItemComponent = PlayerCharacter->GetComponentByClass<UItemComponent>();
		if (ItemComponent)
		{
			ItemComponent->OnInventoryItemListChanged.BindUObject(this, &UDomiInventoryWidget::OnUpdateInventoryItemList);
		}
	}
}

void UDomiInventoryWidget::OnUpdateInventoryItemList()
{
	InventoryAllItems = ItemComponent->GetInventoryDisplayItems();

	for (FItemUISlotData& SlotData : InventoryAllItems)
	{
		if (SlotData.ItemTag.MatchesTag(ItemTags::EquippableItem))
		{
			EquippableItemsMap.Add(SlotData.ItemName, SlotData);
			continue;
		}

		if (SlotData.ItemTag.MatchesTag(ItemTags::ConsumableItem))
		{
			ConsumableItemsMap.Add(SlotData.ItemName, SlotData);
			continue;
		}
		
		if (SlotData.ItemTag.MatchesTag(ItemTags::OtherItem))
    	{
			OtherItemsMap.Add(SlotData.ItemName, SlotData);
			continue;
		}

		ensureMsgf(false, TEXT("%s:: Error Item Tag"), *SlotData.ItemName);
	}

	UpdateInventoryItemList();

}
