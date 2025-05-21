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

void UItemComponent::SwapWeapons()
{
	FGameplayTag MainWeapon = EquipmentSlots.FindRef(FName("WeaponSlot_Main"));
	FGameplayTag SecondaryWeapon = EquipmentSlots.FindRef(FName("WeaponSlot_Secondary"));

	EquipmentSlots[FName("WeaponSlot_Main")] = SecondaryWeapon;
	EquipmentSlots[FName("WeaponSlot_Secondary")] = MainWeapon;

	Debug::Print(TEXT("무기 슬롯을 스왑했습니다."));
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
			return true;
		}
		else
		{
			Debug::Print(FString::Printf(TEXT("인벤토리에 '%s'이 없어 소비 아이템 슬롯 %d에 설정할 수 없습니다."), *ItemTag.ToString(), SlotIndex));
			ConsumableSlots[SlotIndex] = FGameplayTag(); // 슬롯 비움
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
				const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemToUse.GetTagName(), TEXT(""));
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
