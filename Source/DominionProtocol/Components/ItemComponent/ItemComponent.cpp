#include "ItemComponent.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "ItemInventory/ItemData.h"
#include "ItemInventory/BaseItem.h"
#include "Interface/ConsumableItemInterface.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 기본 슬롯 초기화
	EquipmentSlots.Add(FName("WeaponSlot_Primary"), FGameplayTag());
	EquipmentSlots.Add(FName("WeaponSlot_Secondary"), FGameplayTag());
	EquipmentSlots.Add(FName("AccessorySlot_Primary"), FGameplayTag());
	EquipmentSlots.Add(FName("AccessorySlot_Secondary"), FGameplayTag());
	EquipmentSlots.Add(FName("SkillSlot"), FGameplayTag());

	// 소비 아이템 슬롯 초기화
	ConsumableSlots.Add(FName("ConsumableSlot_Primary"), FGameplayTag());
	ConsumableSlots.Add(FName("ConsumableSlot_Secondary"), FGameplayTag());
	ConsumableSlots.Add(FName("ConsumableSlot_Tertiary"), FGameplayTag());
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	//데이터 테이블 할당 체크
	if (ItemDataTable)
	{
		// ItemDataTable의 모든 행을 CachedItemDataMap에 로드
		TArray<FItemData*> ItemDataRows;
		ItemDataTable->GetAllRows<FItemData>(TEXT("ItemComponent::BeginPlay"),ItemDataRows);

		for (FItemData* Row : ItemDataRows)
		{
			if (Row && Row->ItemTag.IsValid())
			{
				CachedItemDataMap.Add(Row->ItemTag, *Row); // FItemData 전체를 값으로 복사하여 저장
			}
		}
		Debug::Print(FString::Printf(TEXT("ItemDataTable loaded and cached %d items."), CachedItemDataMap.Num()));
	}
	else
	{
		Debug::Print(TEXT("Error: ItemDataTable is not assigned in the Editor!"));
	}
	
}

bool UItemComponent::AddItem(FGameplayTag ItemTag, int32 Quantity)
{
	//수량이 0보다 작거나 같으면 리턴
	if (Quantity <= 0)
	{
		return false;
	}
	//캐싱된 맵에서 데이터 불러옴
	const FItemData* ItemData = GetItemDataFromTable(ItemTag);
	if (!ItemData)
	{
		Debug::Print(FString::Printf(TEXT("아이템 태그 '%s'에 해당하는 데이터가 없습니다."), *ItemTag.ToString()));
		return false; // 아이템 데이터 없음
	}

	int32 MaxQuantity = ItemData->MaxItemQuantity;

	// 기존 아이템이 있을 경우
	if (InventoryMap.Contains(ItemTag))
	{
		if (InventoryMap[ItemTag] + Quantity <= MaxQuantity)
		{
			InventoryMap[ItemTag] += Quantity;
			Debug::Print("Item Quantity Added (Within Limit)");
			OnInventoryItemListChanged.Execute();
			return true;
		}
		else
		{
			Debug::Print("Item Quantity Exceeds Max Limit");
			return false;
		}
	}
	else // 새로운 아이템일 경우
	{
		if (Quantity <= MaxQuantity)
		{
			InventoryMap.Add(ItemTag, Quantity);
			Debug::Print("New Item Added");
			OnInventoryItemListChanged.Execute();
			return true;
		}
		else
		{
			Debug::Print("New Item Quantity Exceeds Max Limit");
			return false;
		}
	}
}

bool UItemComponent::RemoveItem(FGameplayTag ItemTag, int32 Quantity)
{
	//아이템 태그가 없거나 수량이 0보다 적으면 리턴
	if (!InventoryMap.Contains(ItemTag) || Quantity <= 0)
	{
		Debug::Print("Invalid Item");
		return false;
	}
	//아이템수량>제거수량 이면 개수 감소
	if (InventoryMap[ItemTag] > Quantity)
	{
		InventoryMap[ItemTag] -= Quantity;
		Debug::Print("Item Quantity Decreased");
		OnInventoryItemListChanged.Execute();
		return true;
	}
	//아이템수량=제거할수량이면 아이템태그 제거
	else if (InventoryMap[ItemTag] == Quantity)
	{
		InventoryMap.Remove(ItemTag);
		Debug::Print("Item Tag Removed");
		OnInventoryItemListChanged.Execute();
		return true;
	}
	//아이템수량<제거수량이면 시도불가
	Debug::Print("Invalid Quantity");
	return false;
}

//아이템 특정 개수 이상 보유 여부 확인
bool UItemComponent::HasItem(FGameplayTag ItemTag, int32 Quantity) const
{
	return InventoryMap.Contains(ItemTag) && InventoryMap[ItemTag] >= Quantity;
}

//아이템 개수 반환
int32 UItemComponent::GetItemQuantity(FGameplayTag ItemTag) const
{
	return InventoryMap.Contains(ItemTag) ? InventoryMap[ItemTag] : 0;
}

//인벤토리 맵 GETTER
const TMap<FGameplayTag, int32>& UItemComponent::GetInventoryMap() const
{
	return InventoryMap;
}

//장비 장착
bool UItemComponent::EquipItem(FName SlotName, FGameplayTag ItemTag)
{
	FName PreSlotName;
	
	for (auto& Pair : EquipmentSlots)
	{
		// 이미 장착 중
		if (Pair.Value == ItemTag)
		{
			PreSlotName = Pair.Key;
			break;
		}
	}

	if (PreSlotName != NAME_None)
	{
		EquipmentSlots[PreSlotName] = FGameplayTag();
		EquipmentSlots[SlotName] = ItemTag;
		OnInventoryEquippedSlotItemsChanged.Execute();
		return false;
	}
	
	//데이터 테이블 캐싱 여부 확인
	if (CachedItemDataMap.IsEmpty())
	{
		Debug::Print(TEXT("ItemComponent: CachedItemDataMap is empty. ItemDataTable might not be loaded or assigned correctly."));
		return false;
	}
	const FItemData* ItemData = GetItemDataFromTable(ItemTag);
	//데이터 체크
	if (!ItemData)
	{
		Debug::Print(FString::Printf(TEXT("아이템 태그 '%s'에 해당하는 데이터가 없습니다."), *ItemTag.ToString()));
		return false;
	}
	EItemType ItemType = ItemData->ItemType;
	if (ItemType == EItemType::Equippable)
	{
		// 슬롯이 존재하는지 확인
		if (EquipmentSlots.Contains(SlotName))
		{
			// 슬롯의 기존 태그 확인
			FGameplayTag CurrentTag = EquipmentSlots[SlotName];

			// 태그가 비어있거나 다르다면
			if (!CurrentTag.IsValid() || CurrentTag != ItemTag)
			{
				// 원래의 태그가 유효하다면 장비 해제 (메시지 출력)
				if (CurrentTag.IsValid())
				{
					Debug::Print(FString::Printf(TEXT("슬롯 '%s'의 '%s' 장비 해제"), *SlotName.ToString(), *CurrentTag.ToString()));
				}

				// 새로운 태그 장착
				EquipmentSlots[SlotName] = ItemTag;
				Debug::Print(FString::Printf(TEXT("슬롯 '%s'에 '%s' 장착"), *SlotName.ToString(), *ItemTag.ToString()));
				OnInventoryEquippedSlotItemsChanged.Execute();
				return true;
			}
			else
			{
				Debug::Print(FString::Printf(TEXT("슬롯 '%s'에 이미 동일한 아이템 '%s'이 장착되어 있습니다."), *SlotName.ToString(), *ItemTag.ToString()));
				return false;
			}
		}
		else
		{
			Debug::PrintError(FString::Printf(TEXT("유효하지 않은 장비 슬롯 이름: '%s'"), *SlotName.ToString()));
			return false;
		}
	}
	else
	{
		Debug::Print(TEXT("아이템의 타입이 무기가 아닙니다"));
		return false;
	}
}


//장비 해제
bool UItemComponent::UnequipItem(FName SlotName)
{
	if (EquipmentSlots.Contains(SlotName))
	{
		EquipmentSlots[SlotName] = FGameplayTag(); // 태그를 비움
		OnInventoryEquippedSlotItemsChanged.Execute();
		return true;
	}
	return false;
}

void UItemComponent::SwapWeapons()
{
	FGameplayTag PrimaryWeapon = EquipmentSlots.FindRef(FName("WeaponSlot_Primary"));
	FGameplayTag SecondaryWeapon = EquipmentSlots.FindRef(FName("WeaponSlot_Secondary"));

	EquipmentSlots[FName("WeaponSlot_Primary")] = SecondaryWeapon;
	EquipmentSlots[FName("WeaponSlot_Secondary")] = PrimaryWeapon;


	Debug::Print(TEXT("무기 슬롯을 스왑했습니다."));
	// 장비 변경 알림
	OnInventoryEquippedSlotItemsChanged.Execute();
}

//슬롯에 장착된 아이템 태그 반환
FGameplayTag UItemComponent::GetEquippedItem(FName SlotName) const
{
	return EquipmentSlots.Contains(SlotName) ? EquipmentSlots[SlotName] : FGameplayTag();
}

FName UItemComponent::GetEquippedItemSlotName(FGameplayTag ItemTag)
{
	for (const auto& SlotData : EquipmentSlots)
	{
		if (SlotData.Value == ItemTag)
		{
			return SlotData.Key;
		}
	}
	
	return NAME_None;
}

//장비 슬롯 맵 GETTER
const TMap<FName, FGameplayTag>& UItemComponent::GetEquipmentSlots() const
{
	return EquipmentSlots;
}

bool UItemComponent::PlaceInSlotConsumableItem(FName SlotName, FGameplayTag ItemTag)
{

	FName PreSlotName;
	
	for (auto& Pair : ConsumableSlots)
	{
		// 이미 장착 중
		if (Pair.Value == ItemTag)
		{
			PreSlotName = Pair.Key;
			break;
		}
	}

	if (PreSlotName != NAME_None)
	{
		ConsumableSlots[PreSlotName] = FGameplayTag();
		ConsumableSlots[SlotName] = ItemTag;
		OnInventoryConsumableSlotItemsChanged.Execute();
		return false;
	}
	
	//데이터 테이블 캐싱 여부 확인
	if (CachedItemDataMap.IsEmpty())
	{
		Debug::Print(TEXT("ItemComponent: CachedItemDataMap is empty. ItemDataTable might not be loaded or assigned correctly."));
		return false;
	}
	const FItemData* ItemData = GetItemDataFromTable(ItemTag);
	//데이터 체크
	if (!ItemData)
	{
		Debug::Print(FString::Printf(TEXT("아이템 태그 '%s'에 해당하는 데이터가 없습니다."), *ItemTag.ToString()));
		return false;
	}
	EItemType ItemType = ItemData->ItemType;
	if (ItemType == EItemType::Consumable)
	{
		// 슬롯이 존재하는지 확인
		if (ConsumableSlots.Contains(SlotName))
		{
			// 슬롯의 기존 태그 확인
			FGameplayTag CurrentTag = ConsumableSlots[SlotName];

			// 태그가 비어있거나 다르다면
			if (!CurrentTag.IsValid() || CurrentTag != ItemTag)
			{
				// 원래의 태그가 유효하다면 장비 해제 (메시지 출력)
				if (CurrentTag.IsValid())
				{
					Debug::Print(FString::Printf(TEXT("슬롯 '%s'의 '%s' 등록 해제"), *SlotName.ToString(), *CurrentTag.ToString()));
				}

				// 새로운 태그 장착
				ConsumableSlots[SlotName] = ItemTag;
				Debug::Print(FString::Printf(TEXT("슬롯 '%s'에 '%s' 등록"), *SlotName.ToString(), *ItemTag.ToString()));
				OnInventoryConsumableSlotItemsChanged.Execute();
				return true;
			}
			else
			{
				Debug::Print(FString::Printf(TEXT("슬롯 '%s'에 이미 동일한 아이템 '%s'이 등록 되어 있습니다."), *SlotName.ToString(), *ItemTag.ToString()));
				return false;
			}
		}
		else
		{
			Debug::PrintError(FString::Printf(TEXT("유효하지 않은 소모품 슬롯 이름: '%s'"), *SlotName.ToString()));
			return false;
		}
	}
	else
	{
		Debug::Print(TEXT("아이템의 타입이 소모품이 아닙니다"));
		return false;
	}
}

bool UItemComponent::RemoveFromSlotConsumableItemSlot(FName SlotName)
{
	if (ConsumableSlots.Contains(SlotName))
	{
		ConsumableSlots[SlotName] = FGameplayTag(); // 태그를 비움
		OnInventoryConsumableSlotItemsChanged.Execute();
		return true;
	}
	return false;
}

FName UItemComponent::GetRegisteredSlotName(FGameplayTag ItemTag)
{
	for (const auto& SlotData : ConsumableSlots)
	{
		if (SlotData.Value == ItemTag)
		{
			return SlotData.Key;
		}
	}

	for (const auto& SlotData : EquipmentSlots)
	{
		if (SlotData.Value == ItemTag)
		{
			return SlotData.Key;
		}
	}
	
	return NAME_None;
}

//소비아이템 사용
void UItemComponent::UseConsumableItem(FName SlotName, FGameplayTag ConsumableItemTag)
{

	FGameplayTag ItemToUse;
	
	if (SlotName != NAME_None)
	{
		ItemToUse = ConsumableSlots[SlotName];
	}
	
	if (ConsumableItemTag != FGameplayTag())
	{
		ItemToUse = ConsumableItemTag;
	}
	
	if (ItemToUse.IsValid())
	{
		if (HasItem(ItemToUse, 1)) // 인벤토리에 아이템이 있는지 확인 (수량 1 이상)
		{
			const FItemData* ItemData = GetItemDataFromTable(ItemToUse); // 헬퍼 함수 사용

			if (ItemData && ItemData->ItemClass)
			{
				// 임시 아이템 액터 생성
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				ABaseItem* ConsumableActor = GetWorld()->SpawnActor<ABaseItem>(ItemData->ItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

				if (ConsumableActor && ConsumableActor->Implements<UConsumableItemInterface>())
				{
					// Consume 인터페이스 실행 (소비 주체 전달)
					IConsumableItemInterface::Execute_Consume(ConsumableActor, GetOwner());
					OnInventoryItemListChanged.Execute();

					// 소비 후 임시 액터 파괴 (Consume_Implementation에서 RemoveItem이 호출되었을 것으로 가정)
					ConsumableActor->Destroy();
				}
				else
				{
					Debug::PrintError(FString::Printf(TEXT("소비 아이템 '%s'은 IConsumableItemInterface를 구현하지 않습니다."), *ItemToUse.ToString()));
				}
			}
			else
			{
				Debug::PrintError(FString::Printf(TEXT("아이템 태그 '%s'에 해당하는 아이템 데이터 또는 클래스를 찾을 수 없습니다."), *ItemToUse.ToString()));
			}
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("소비 아이템 슬롯 %s의 '%s'을 사용하려 했으나 인벤토리에 부족합니다."), *SlotName.ToString(), *ItemToUse.ToString()));
			// 인벤토리에 없으므로 슬롯을 다시 비움
			ConsumableSlots[SlotName] = FGameplayTag();
			OnInventoryConsumableSlotItemsChanged.Execute();
		}
	}
	else
	{
		Debug::Print(FString::Printf(TEXT("소비 아이템 슬롯 %s이 비어 있습니다."), *SlotName.ToString()));
	}
	
}

const TMap<FName, FGameplayTag>& UItemComponent::GetConsumableSlots() const
{
	return ConsumableSlots;
}

//포션 부스트 적용
void UItemComponent::ApplyPotionBoost()
{
	if (bIsPotionBoostApplied)
	{
		Debug::Print(TEXT("이미 강화됨"));
		return;
	}
	
	TMap<FGameplayTag, int32> NewInventoryMap;
	//인벤토리 업데이트
	for (const auto& Pair : InventoryMap)
	{
		const FGameplayTag& CurrentTag = Pair.Key;
		const int32 Quantity = Pair.Value;

		// 만약 현재 태그가 'Item.Consumable.Potion
		if (CurrentTag == ItemTags::Potion)
		{
			// 새로운 태그 'Item.Consumable.Potion.Boosted'로 수량을 옮김
			NewInventoryMap.Add(ItemTags::PotionBoosted, Quantity);
			Debug::Print(TEXT("Inventory Changed"));
		}
		else
		{
			// 다른 아이템은 그대로 유지
			NewInventoryMap.Add(CurrentTag, Quantity);
		}
	}
	InventoryMap = NewInventoryMap;
	OnInventoryItemListChanged.Execute();

	//소비슬롯 업데이트
	for (auto& Pair: ConsumableSlots)
	{
		FGameplayTag& SlotTag = Pair.Value; // 참조로 가져와서 직접 수정

		// 만약 현재 슬롯 태그가 'Item.Consumable.Potion.Health'라면
		if (SlotTag == ItemTags::Potion)
		{
			// 새로운 태그 'Item.Consumable.Potion.Health.Boosted'로 변경
			SlotTag = ItemTags::PotionBoosted;
			OnInventoryConsumableSlotItemsChanged.Execute();
			Debug::Print(FString::Printf(TEXT("Consumable Slot %s: Changed to %s"), *Pair.Key.ToString(), *SlotTag.ToString()));
		}
	}
	Debug::Print(TEXT("Potion boost applied! All applicable potions have been upgraded."));

}

//UI용 함수
// 인벤토리의 모든 아이템 정보를 FItemUISlotData 배열로 반환
TArray<FItemUISlotData> UItemComponent::GetInventoryDisplayItems() const
{
	TArray<FItemUISlotData> DisplayItems;

	// InventoryMap을 순회하면서 각 아이템에 대한 FItemUISlotData를 생성
	for (const auto& Pair : InventoryMap)
	{
		const FGameplayTag& ItemTag = Pair.Key;
		const int32 CurrentQuantity = Pair.Value;

		const FItemData* ItemData = GetItemDataFromTable(ItemTag);
		if (ItemData)
		{
			FItemUISlotData UISlotData;
			UISlotData.ItemTag = ItemTag;
			UISlotData.ItemIcon = ItemData->ItemIcon;
			UISlotData.ItemName = ItemData->ItemName; 
			UISlotData.ItemDescription = ItemData->ItemDescription;
			UISlotData.CurrentQuantity = CurrentQuantity;//현재수량만 맵의 밸류에서 가져옴
			UISlotData.MaxStackQuantity = ItemData->MaxItemQuantity;
			UISlotData.ItemType = ItemData->ItemType;

			DisplayItems.Add(UISlotData);
		}
	}
	return DisplayItems;
}

// 장비 슬롯의 모든 아이템 정보를 FItemUISlotData 맵으로 반환
TMap<FName, FItemUISlotData> UItemComponent::GetEquippedDisplayItems() const
{
	TMap<FName, FItemUISlotData> EquippedDisplayItems;

	for (const auto& Pair : EquipmentSlots)
	{
		const FName& SlotName = Pair.Key;
		const FGameplayTag& EquippedTag = Pair.Value;

		if (EquippedTag.IsValid())
		{
			const FItemData* ItemData = GetItemDataFromTable(EquippedTag);
			if (ItemData)
			{
				FItemUISlotData UISlotData;
				UISlotData.ItemTag = EquippedTag;
				UISlotData.ItemIcon = ItemData->ItemIcon;
				UISlotData.ItemName = ItemData->ItemName;
				UISlotData.ItemDescription = ItemData->ItemDescription;
				UISlotData.CurrentQuantity = 1; // 장비는 보통 1개이므로
				UISlotData.MaxStackQuantity = 1; // 장비는 보통 1개이므로
				UISlotData.ItemType = ItemData->ItemType;

				EquippedDisplayItems.Add(SlotName, UISlotData);
			}
		}
	}
	return EquippedDisplayItems;
}

// 소비 아이템 슬롯의 모든 아이템 정보를 FItemUISlotData 배열로 반환
TMap<FName, FItemUISlotData> UItemComponent::GetConsumableDisplayItems() const
{
	TMap<FName, FItemUISlotData> ConsumableDisplayItems;

	for (const auto& Pair : ConsumableSlots)
	{
		const FName& SlotName = Pair.Key;
		const FGameplayTag& ConsumableTag = Pair.Value;
		
		if (ConsumableTag.IsValid())
		{
			const FItemData* ItemData = GetItemDataFromTable(ConsumableTag);
			if (ItemData)
			{
				FItemUISlotData UISlotData;
				UISlotData.ItemTag = ConsumableTag;
				UISlotData.ItemIcon = ItemData->ItemIcon;
				UISlotData.ItemName = ItemData->ItemName;
				UISlotData.ItemDescription = ItemData->ItemDescription;
				UISlotData.CurrentQuantity = GetItemQuantity(ConsumableTag); // 인벤토리에서 실제 수량 가져옴
				UISlotData.MaxStackQuantity = ItemData->MaxItemQuantity;
				UISlotData.ItemType = ItemData->ItemType;

				ConsumableDisplayItems.Add(SlotName, UISlotData);
			}
		}
		else
		{
			// 빈 슬롯도 표시하려면 유효하지 않은 ItemTag를 가진 FItemUISlotData를 추가할 수 있습니다.
			// 예: ConsumableDisplayItems.Add(FItemUISlotData());
		}
	}
	return ConsumableDisplayItems;
}

// 캐싱된 맵에서 FItemData를 로드하는 헬퍼 함수
const FItemData* UItemComponent::GetItemDataFromTable(FGameplayTag ItemTag) const
{
	const FItemData* ItemData = CachedItemDataMap.Find(ItemTag);
	if (!ItemData)
	{
		Debug::PrintError(FString::Printf(TEXT("ItemComponent: No cached data found for ItemTag '%s'."), *ItemTag.ToString()));
	}
	return ItemData;
}

//치트매니저 전용
void UItemComponent::AddAllItemsToInventoryMaxQuantity()
{
	if (CachedItemDataMap.IsEmpty())
	{
		Debug::PrintError(TEXT("AddAllItemsToInventoryMaxQuantity: CachedItemDataMap is empty. ItemDataTable might not be loaded or assigned correctly."));
		return;
	}

	int32 AddedCount = 0;
	// 캐싱된 ItemDataMap을 순회하며 모든 아이템을 인벤토리에 추가
	for (const auto& Pair : CachedItemDataMap)
	{
		const FGameplayTag& ItemTag = Pair.Key;
		const FItemData& ItemData = Pair.Value;

		// 아이템의 최대 수량만큼 추가 (AddItem 함수가 내부적으로 수량을 관리)
		if (AddItem(ItemTag, ItemData.MaxItemQuantity))
		{
			AddedCount++;
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("Failed to add %s to inventory with max quantity."), *ItemTag.ToString()));
		}
	}

	if (AddedCount > 0)
	{
		Debug::Print(FString::Printf(TEXT("Added %d unique items to inventory with max quantity."), AddedCount));
		// 인벤토리 목록이 변경되었음을 UI에 알림
		OnInventoryItemListChanged.ExecuteIfBound();
	}
	else
	{
		Debug::Print(TEXT("No items were added or all items already at max quantity."));
	}
}
