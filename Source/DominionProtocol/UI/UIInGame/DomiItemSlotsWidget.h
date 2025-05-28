// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/ItemUISlotData.h"
#include "Blueprint/UserWidget.h"
#include "DomiItemSlotsWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiItemSlotsWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	void BindItemSlotsDelegate();

	UFUNCTION()
	void OnUpdateEquippableSlotItems();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquippableSlotItems();

	UFUNCTION()
	void OnUpdateConsumableSlotItems();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateConsumableSlotItems();
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FItemUISlotData> InventoryEquippableSlotItems;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> InventoryConsumableSlotItems;
	
	UPROPERTY()
	TObjectPtr<class UItemComponent> ItemComponent;
};
