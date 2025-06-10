// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewInventoryWidget.h"

#include "Components/ItemComponent/ItemComponent.h"

void UNewInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInventoryDelegates();
}

void UNewInventoryWidget::BindInventoryDelegates()
{
	AActor* PlayerCharacter = GetOwningPlayerPawn();
	if (PlayerCharacter)
	{
		ItemComponent = PlayerCharacter->GetComponentByClass<UItemComponent>();
		if (ItemComponent)
		{
			ItemComponent->OnInventoryItemListChanged.BindUObject(this, &UNewInventoryWidget::OnUpdateInventoryItemList);
		}
	}
}

void UNewInventoryWidget::OnUpdateInventoryItemList()
{
	InventoryAllItems = ItemComponent->GetInventoryDisplayItems();
	
	TArray<FItemUISlotData> TempWeaponItemsArray;
	TArray<FItemUISlotData> TempAccessoryItemsArray;
	TArray<FItemUISlotData> TempSkillItemsArray;
	TArray<FItemUISlotData> TempConsumableItemsArray;
	TArray<FItemUISlotData> TempOtherItemsArray;
		
	for (FItemUISlotData& SlotData : InventoryAllItems)
	{

		
		if (SlotData.ItemTag.MatchesTag(ItemTags::WeaponItem))
		{
			TempWeaponItemsArray.Add(SlotData);
			continue;
		}

		if (SlotData.ItemTag.MatchesTag(ItemTags::AccessoryItem))
		{
			TempAccessoryItemsArray.Add(SlotData);
			continue;
		}

		if (SlotData.ItemTag.MatchesTag(ItemTags::SkillItem))
		{
			TempSkillItemsArray.Add(SlotData);
			continue;
		}

		if (SlotData.ItemTag.MatchesTag(ItemTags::ConsumableItem))
		{
			TempConsumableItemsArray.Add(SlotData);
			continue;
		}
		
		if (SlotData.ItemTag.MatchesTag(ItemTags::OtherItem))
		{
			TempOtherItemsArray.Add(SlotData);
			continue;
		}

		ensureMsgf(false, TEXT("%s:: Error Item Tag"), *SlotData.ItemName);
	}


	WeaponItemsItems = TempWeaponItemsArray;
	AccessoryItemsItems = TempAccessoryItemsArray;
	SkillItemsItems = TempSkillItemsArray;
	ConsumableItemsArray = TempConsumableItemsArray;
	OtherItemsArray = TempOtherItemsArray;

	UpdateInventoryItemList();
}