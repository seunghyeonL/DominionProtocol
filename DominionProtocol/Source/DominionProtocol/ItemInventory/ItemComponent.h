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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, int32> InventoryMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	UDataTable* ItemDataTable;
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
		
};
