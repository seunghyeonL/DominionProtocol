#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "ItemInventory/ItemUISlotData.h"
#include "Engine/DataTable.h"
#include "ItemComponent.generated.h"

DECLARE_DELEGATE(FOnInventoryItemListChanged)
DECLARE_DELEGATE(FOnInventoryEquippedSlotItemsChanged)
DECLARE_DELEGATE(FOnInventoryConsumableSlotItemsChanged)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemComponent();

	FOnInventoryItemListChanged OnInventoryItemListChanged;
	FOnInventoryEquippedSlotItemsChanged OnInventoryEquippedSlotItemsChanged;
	FOnInventoryConsumableSlotItemsChanged OnInventoryConsumableSlotItemsChanged;
	

protected:
	virtual void BeginPlay() override;

	//인벤토리
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, int32> InventoryMap;
	
	//데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	UDataTable* ItemDataTable;

	//장비슬롯
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FGameplayTag> EquipmentSlots;

	// 소비 아이템 슬롯
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Consumable")
	TArray<FGameplayTag> ConsumableSlots;

	// 게임 시작 시 ItemDataTable의 모든 FItemData를 로드해 사용
	UPROPERTY(Transient) // 런타임에만 존재하고 저장되지 않음
	TMap<FGameplayTag, FItemData> CachedItemDataMap;

	// 포션 부스트가 적용되었는지 추적하는 변수
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Consumable|Potion")
	bool bIsPotionBoostApplied;
public:	
	// 아이템 추가
	UFUNCTION(BlueprintCallable)
	bool AddItem(FGameplayTag ItemTag, int32 Quantity = 1);

	// 아이템 제거
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FGameplayTag ItemTag, int32 Quantity = 1);

	// 아이템 보유 여부 확인
	UFUNCTION(BlueprintPure)
	bool HasItem(FGameplayTag ItemTag, int32 Quantity = 1) const;

	// 특정 아이템 수량 반환
	UFUNCTION(BlueprintPure)
	int32 GetItemQuantity(FGameplayTag ItemTag) const;

	// 인벤토리 정보 반환 (위젯에 전달)
	UFUNCTION(BlueprintPure)
	const TMap<FGameplayTag, int32>& GetInventoryMap() const;
		
	// 장비 장착
	UFUNCTION(BlueprintCallable)
	bool EquipItem(FName SlotName, FGameplayTag ItemTag);

	// 장비 해제
	UFUNCTION(BlueprintCallable)
	bool UnequipItem(FName SlotName);

	// 무기 슬롯 스왑
	UFUNCTION(BlueprintCallable)
	void SwapWeapons();

	// 특정 슬롯에 장착된 아이템 태그 반환
	UFUNCTION(BlueprintPure)
	FGameplayTag GetEquippedItem(FName SlotName) const;

	// 장비 슬롯 정보 반환 (위젯에 전달)
	UFUNCTION(BlueprintPure)
	const TMap<FName, FGameplayTag>& GetEquipmentSlots() const;

	//소비아이템 등록
	UFUNCTION(BlueprintCallable)
	bool SetConsumableItem(int32 SlotIndex, FGameplayTag ItemTag);

	// 소비 아이템 사용
	UFUNCTION(BlueprintCallable)
	void UseConsumableItem(int32 SlotIndex);

	// 특정 소비 아이템 슬롯 정보 반환
	UFUNCTION(BlueprintPure)
	FGameplayTag GetConsumableItem(int32 SlotIndex) const;

	// 모든 소비 아이템 슬롯 정보 반환
	UFUNCTION(BlueprintPure)
	const TArray<FGameplayTag>& GetConsumableSlots() const;
	
	//UI 프로퍼티 추가

	//UI용 함수 추가
	 // 인벤토리의 모든 아이템 정보를 FItemUISlotData 배열로 반환
	UFUNCTION(BlueprintPure, Category = "Inventory|UI")
	TArray<FItemUISlotData> GetInventoryDisplayItems() const;

	// 장비 슬롯의 모든 아이템 정보를 FItemUISlotData 배열로 반환
	UFUNCTION(BlueprintPure, Category = "Equipment|UI")
	TMap<FName, FItemUISlotData> GetEquippedDisplayItems() const;

	// 소비 아이템 슬롯의 모든 아이템 정보를 FItemUISlotData 배열로 반환
	UFUNCTION(BlueprintPure, Category = "Consumable|UI")
	TArray<FItemUISlotData> GetConsumableDisplayItems() const;

	// 포션 태그를 부스트된 태그로 변경
	UFUNCTION(BlueprintCallable, Category = "Consumable|Potion")
	void ApplyPotionBoost();

	//치트 매니저에서 사용할 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory|Cheat")
	void AddAllItemsToInventoryMaxQuantity();

private:
	//캐싱된 ItemDataTable에서 FItemData를 로드하는 헬퍼 함수
	const FItemData* GetItemDataFromTable(FGameplayTag ItemTag) const;
};
