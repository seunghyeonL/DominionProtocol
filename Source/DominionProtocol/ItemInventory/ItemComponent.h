#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "ItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemComponent();

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

	// 특정 슬롯에 장착된 아이템 태그 반환
	UFUNCTION(BlueprintPure)
	FGameplayTag GetEquippedItem(FName SlotName) const;

	// 장비 슬롯 정보 반환 (위젯에 전달)
	UFUNCTION(BlueprintPure)
	const TMap<FName, FGameplayTag>& GetEquipmentSlots() const;

};
