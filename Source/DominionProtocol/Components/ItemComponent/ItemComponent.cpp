#include "ItemComponent.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "ItemInventory/ItemData.h"
#include "ItemInventory/BaseItem.h"
#include "Interface/ConsumableItemInterface.h"
#include "Kismet/GameplayStatics.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 기본 슬롯 초기화
	EquipmentSlots.Add(FName("WeaponSlot_Main"), FGameplayTag());
	EquipmentSlots.Add(FName("WeaponSlot_Secondary"), FGameplayTag());

	// 소비 아이템 슬롯 초기화
	ConsumableSlots.Init(FGameplayTag(), 3);
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
			//OnInventoryUpdated.Broadcast(); // 인벤토리 변경 알림
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
			//OnInventoryUpdated.Broadcast(); // 인벤토리 변경 알림
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
		//OnInventoryUpdated.Broadcast();
		return true;
	}
	//아이템수량=제거할수량이면 아이템태그 제거
	else if (InventoryMap[ItemTag] == Quantity)
	{
		InventoryMap.Remove(ItemTag);
		Debug::Print("Item Tag Removed");
		//OnInventoryUpdated.Broadcast();
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
	if (ItemType == EItemType::Weapon)
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
				//OnEquipmentUpdated.Broadcast(); // 장비 변경 알림
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
		//OnEquipmentUpdated.Broadcast(); // 장비 변경 알림
		return true;
	}
	return false;
}

void UItemComponent::SwapWeapons()
{
	FGameplayTag MainWeapon = EquipmentSlots.FindRef(FName("WeaponSlot_Main"));
	FGameplayTag SecondaryWeapon = EquipmentSlots.FindRef(FName("WeaponSlot_Secondary"));

	EquipmentSlots[FName("WeaponSlot_Main")] = SecondaryWeapon;
	EquipmentSlots[FName("WeaponSlot_Secondary")] = MainWeapon;

	OnSwapWeapons.Execute();

	Debug::Print(TEXT("무기 슬롯을 스왑했습니다."));
	//OnEquipmentUpdated.Broadcast(); // 장비 변경 알림
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

//소비아이템 등록
bool UItemComponent::SetConsumableItem(int32 SlotIndex, FGameplayTag ItemTag)
{
	if (SlotIndex >= 0 && SlotIndex < ConsumableSlots.Num())
	{
		if (HasItem(ItemTag, 1))
		{
			ConsumableSlots[SlotIndex] = ItemTag;
			Debug::Print(FString::Printf(TEXT("소비 아이템 슬롯 %d에 '%s'을 설정했습니다."), SlotIndex, *ItemTag.ToString()));
			//OnConsumableSlotsUpdated.Broadcast();
			return true;
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("인벤토리에 '%s'이 없어 소비 아이템 슬롯 %d에 설정할 수 없습니다."), *ItemTag.ToString(), SlotIndex));
			ConsumableSlots[SlotIndex] = FGameplayTag(); // 슬롯 비움
			//OnConsumableSlotsUpdated.Broadcast();
			return false;
		}
	}
	else
	{
		Debug::PrintError(FString::Printf(TEXT("유효하지 않은 소비 아이템 슬롯 인덱스: %d (최대: %d)"), SlotIndex, ConsumableSlots.Num() - 1));
		return false;
	}
}

//소비아이템 사용
void UItemComponent::UseConsumableItem(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < ConsumableSlots.Num())
	{
		FGameplayTag ItemToUse = ConsumableSlots[SlotIndex];
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
				Debug::Print(FString::Printf(TEXT("소비 아이템 슬롯 %d의 '%s'을 사용하려 했으나 인벤토리에 부족합니다."), SlotIndex, *ItemToUse.ToString()));
				// 인벤토리에 없으므로 슬롯을 다시 비움
				ConsumableSlots[SlotIndex] = FGameplayTag();
				//OnConsumableSlotsUpdated.Broadcast();//소비슬롯 변경알림
			}
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("소비 아이템 슬롯 %d이 비어 있습니다."), SlotIndex));
		}
	}
	else
	{
		Debug::PrintError(FString::Printf(TEXT("유효하지 않은 소비 아이템 슬롯 인덱스: %d (최대: %d)"), SlotIndex, ConsumableSlots.Num() - 1));
	}
}

FGameplayTag UItemComponent::GetConsumableItem(int32 SlotIndex) const
{
	if (SlotIndex >= 0 && SlotIndex < ConsumableSlots.Num())
	{
		return ConsumableSlots[SlotIndex];
	}
	else
	{
		Debug::PrintError(FString::Printf(TEXT("유효하지 않은 소비 아이템 슬롯 인덱스 요청: %d (최대: %d)"), SlotIndex, ConsumableSlots.Num() - 1));
		return FGameplayTag();
	}
}

const TArray<FGameplayTag>& UItemComponent::GetConsumableSlots() const
{
	return ConsumableSlots;
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
TArray<FItemUISlotData> UItemComponent::GetConsumableDisplayItems() const
{
	TArray<FItemUISlotData> ConsumableDisplayItems;

	for (const FGameplayTag& ItemTag : ConsumableSlots)
	{
		if (ItemTag.IsValid())
		{
			const FItemData* ItemData = GetItemDataFromTable(ItemTag);
			if (ItemData)
			{
				FItemUISlotData UISlotData;
				UISlotData.ItemTag = ItemTag;
				UISlotData.ItemIcon = ItemData->ItemIcon;
				UISlotData.ItemName = ItemData->ItemName;
				UISlotData.ItemDescription = ItemData->ItemDescription;
				UISlotData.CurrentQuantity = GetItemQuantity(ItemTag); // 인벤토리에서 실제 수량 가져옴
				UISlotData.MaxStackQuantity = ItemData->MaxItemQuantity;
				UISlotData.ItemType = ItemData->ItemType;

				ConsumableDisplayItems.Add(UISlotData);
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