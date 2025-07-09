#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "ItemInventory/ItemUISlotData.h"
#include "Engine/DataTable.h"
#include "ItemComponent.generated.h"

struct FSkillComponentInitializeData;
class USoundCue;
class UItemInstanceSubsystem;

DECLARE_MULTICAST_DELEGATE(FOnInventoryItemListChanged)
DECLARE_MULTICAST_DELEGATE(FOnInventoryEquippedSlotItemsChanged)
DECLARE_MULTICAST_DELEGATE(FOnInventoryConsumableSlotItemsChanged)
DECLARE_DELEGATE_OneParam(FOnPrimaryWeaponChangedForSkillSet, const FSkillComponentInitializeData&)
DECLARE_DELEGATE_OneParam(FOnPrimaryWeaponChangedForWeaponVisibility, const FGameplayTag&)
DECLARE_DELEGATE(FOnPrimaryWeaponChangedForAttackPowerSet)
DECLARE_DELEGATE(FOnSecondaryWeaponChangedForSubAttackPowerSet)
DECLARE_DELEGATE_OneParam(FOnMagicSkillChanged, const FSkillComponentInitializeData&)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAddItem, FGameplayTag, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUseItem, FGameplayTag)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemComponent();

	FOnInventoryItemListChanged OnInventoryItemListChanged;
	FOnInventoryEquippedSlotItemsChanged OnInventoryEquippedSlotItemsChanged;
	FOnInventoryConsumableSlotItemsChanged OnInventoryConsumableSlotItemsChanged;
	FOnPrimaryWeaponChangedForSkillSet OnPrimaryWeaponChangedForSkillSet;
	FOnPrimaryWeaponChangedForWeaponVisibility OnPrimaryWeaponChangedForWeaponVisibility;
	FOnPrimaryWeaponChangedForAttackPowerSet OnPrimaryWeaponChangedForAttackPowerSet;
	FOnSecondaryWeaponChangedForSubAttackPowerSet OnSecondaryWeaponChangedForSubAttackPowerSet;
	FOnMagicSkillChanged OnMagicSkillChanged;
	FOnAddItem OnAddItem;
	FOnUseItem OnUseItem;

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
	TMap<FName, FGameplayTag> ConsumableSlots;

	// 게임 시작 시 ItemDataTable의 모든 FItemData를 로드해 사용
	UPROPERTY(Transient) // 런타임에만 존재하고 저장되지 않음
	TMap<FGameplayTag, FItemData> CachedItemDataMap;

	//사운드
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* UnEquipSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* ConsumeSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* SwapSound;

	// 포션 부스트가 몇번 적용되었는지 추적하는 변수
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Consumable|Potion")
	int32 PotionBoostLevel=0;

	void SetTagToSlot(FName SlotName, FGameplayTag ItemTag);
public:
	void SetSkillFromItemWhenLevelChanged();
	
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

	UFUNCTION(BlueprintCallable)
	FName GetEquippedItemSlotName(FGameplayTag ItemTag);

	// 장비 슬롯 정보 반환 (위젯에 전달)
	UFUNCTION(BlueprintPure)
	const TMap<FName, FGameplayTag>& GetEquipmentSlots() const;

	// 장착된 무기 계수 반환 (공격력 계산)
	float GetPrimaryWeaponCoefficient();
	float GetSecondaryWeaponCoefficient();

	//소비아이템 등록
	UFUNCTION(BlueprintCallable)
	bool PlaceInSlotConsumableItem(FName SlotName, FGameplayTag ItemTag);

	UFUNCTION(BlueprintCallable)
	bool RemoveFromSlotConsumableItemSlot(FName SlotName);

	UFUNCTION(BlueprintCallable)
	FName GetRegisteredSlotName(FGameplayTag ItemTag);

	// 소비 아이템 사용
	UFUNCTION(BlueprintCallable)
	void UseConsumableItem(FName SlotName, FGameplayTag ConsumableItemTag = FGameplayTag());

	// 회복 포션 회복(균열 호출)
	UFUNCTION(BlueprintCallable)
	void RestorePotion();
	
	// 모든 소비 아이템 슬롯 정보 반환
	UFUNCTION(BlueprintPure)
	const TMap<FName, FGameplayTag>& GetConsumableSlots() const;
	
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
	TMap<FName, FItemUISlotData> GetConsumableDisplayItems() const;

	// 포션 태그를 부스트된 태그로 변경
	UFUNCTION(BlueprintCallable, Category = "Consumable|Potion")
	void ApplyPotionBoost(int32 BoostAmount=1);

	UFUNCTION(BlueprintCallable, Category = "Consumable|Potion")
	FGameplayTag GetPotionTagByLevel(int32 Level)const;

	//치트 매니저에서 사용할 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory|Cheat")
	void AddAllItemsToInventoryMaxQuantity();

	// ItemInstanceSubsystem 함수
	FORCEINLINE void SetInventoryMap(TMap<FGameplayTag, int32>& NewMap) { InventoryMap = NewMap; }

	FORCEINLINE void SetEquipmentSlots(TMap<FName, FGameplayTag>& NewMap) { EquipmentSlots = NewMap; }

	FORCEINLINE void SetConsumableSlots(TMap<FName, FGameplayTag>& NewMap) { ConsumableSlots = NewMap; }

	FORCEINLINE void SetPotionBoostLevel(int32 NewInt) { PotionBoostLevel = NewInt; }

	// ItemInstanceSubsystem에 정보 전달 위한 함수
	void UpdateItemInstanceSubsystem();

	void DelegateExecuter();
	
private:
	//캐싱된 ItemDataTable에서 FItemData를 로드하는 헬퍼 함수
	const FItemData* GetItemDataFromTable(FGameplayTag ItemTag) const;
	
	//사운드 재생 함수
	void PlayEquipSound();
	void PlayUnEquipSound();
	void PlayConsumeSound();
	void PlaySwapSound();

	//소비 애니메이션 VFX 재생
	void PlayConsumeVFXAndAnimation(const FItemData* ConsumedItemData);

	UPROPERTY()
	TObjectPtr<UItemInstanceSubsystem> ItemInstanceSubsystem;
};
