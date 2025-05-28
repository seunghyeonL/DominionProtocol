// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/ItemUISlotData.h"
#include "Blueprint/UserWidget.h"
#include "DomiInventoryWidget.generated.h"

UENUM(BlueprintType)
enum class ELastDisplayArray : uint8
{
	AllItemsArray				UMETA(DisplayName="AllItemsArray"),
	EquippableItemsArray		UMETA(DisplayName="EquippableItemsArray"),
	ConsumableItemsArray		UMETA(DisplayName="ConsumableItemsArray"),
	OtherItemsArray				UMETA(DisplayName="OtherItemsArray"),
};


UCLASS()
class DOMINIONPROTOCOL_API UDomiInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void BindInventoryDelegates();

	UFUNCTION()
	void OnUpdateInventoryItemList();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateInventoryItemList();
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> InventoryAllItems;

	UPROPERTY()
	TObjectPtr<class UItemComponent> ItemComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> EquippableItemsArray;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> ConsumableItemsArray;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> OtherItemsArray;

	UPROPERTY(BlueprintReadWrite)
	ELastDisplayArray LastDisplayArray = ELastDisplayArray::AllItemsArray;
};
