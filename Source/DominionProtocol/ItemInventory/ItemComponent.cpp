#include "ItemInventory/ItemComponent.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "ItemData.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 기본 슬롯 초기화
	EquipmentSlots.Add(FName("WeaponSlot_Main"), FGameplayTag());
	EquipmentSlots.Add(FName("WeaponSlot_Secondary"), FGameplayTag());
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	//데이터 테이블 할당 체크
	if (!ItemDataTable)
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
	//데이터 테이블 체크
	if (ItemDataTable)
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemTag.GetTagName(), TEXT(""));
		//데이터 체크
		if (ItemData)
		{
			//최대수량 값 가져오기
			int32 MaxQuantity = ItemData->MaxItemQuantity;
			//동일 아이템 존재 여부 확인
			if (InventoryMap.Contains(ItemTag))
			{
				if (InventoryMap[ItemTag] + Quantity <= MaxQuantity)
				{
					InventoryMap[ItemTag] += Quantity;
					Debug::Print("Item Quantity Added (Within Limit)");
					return true;
				}
				else
				{
					Debug::Print("Item Quantity Exceeds Max Limit");
					return false;
				}
			}
			else
			{
				if (Quantity <= MaxQuantity)
				{
					InventoryMap.Add(ItemTag, Quantity);
					Debug::Print("New Item Added");
					return true;
				}
				else
				{
					Debug::Print("New Item Quantity Exceeds Max Limit");
					return false;
				}
			}
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("아이템 태그 '%s'에 해당하는 데이터가 테이블에 없습니다."), *ItemTag.ToString()));
			return false;
		}
	}
	else
	{
		Debug::Print(TEXT("아이템 데이터 테이블이 할당되지 않았습니다."));
		return false;
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
		return true;
	}
	//아이템수량=제거할수량이면 아이템태그 제거
	else if (InventoryMap[ItemTag] == Quantity)
	{
		InventoryMap.Remove(ItemTag);
		Debug::Print("Item Tag Removed");
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
	//데이터 테이블 체크
	if (ItemDataTable)
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemTag.GetTagName(), TEXT(""));
		//데이터 체크
		if (ItemData)
		{
			EItemType ItemType = ItemData->ItemType;
			if (ItemType == EItemType::Weapon)
			{
				EquipmentSlots.Add(SlotName, ItemTag);
				return true;
			}
			else
			{
				Debug::Print(TEXT("아이템의 타입이 무기가 아닙니다"));
				return false;
			}
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("아이템 태그 '%s'에 해당하는 데이터가 테이블에 없습니다."), *ItemTag.ToString()));
			return false;
		}
	}
	else
	{
		Debug::Print(TEXT("아이템 데이터 테이블이 할당되지 않았습니다."));
		return false;
	}
}

//장비 해제
bool UItemComponent::UnequipItem(FName SlotName)
{
	if (EquipmentSlots.Contains(SlotName))
	{
		EquipmentSlots[SlotName] = FGameplayTag(); // 태그를 비움
		return true;
	}
	return false;
}

//슬롯에 장착된 아이템 태그 반환
FGameplayTag UItemComponent::GetEquippedItem(FName SlotName) const
{
	return EquipmentSlots.Contains(SlotName) ? EquipmentSlots[SlotName] : FGameplayTag();
}

//장비 슬롯 맵 GETTER
const TMap<FName, FGameplayTag>& UItemComponent::GetEquipmentSlots() const
{
	return EquipmentSlots;
}