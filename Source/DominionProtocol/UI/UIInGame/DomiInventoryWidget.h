// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiInventoryWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void BindInventoryDelegates();

	UFUNCTION()
	void UpdateInventoryItemList();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUpdateInventoryItemList();

	UFUNCTION()
	void UpdateEquippedSlotItems();

	UFUNCTION()
	void UpdateConsumableSlotItems();


protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> InventoryAllItems;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FItemUISlotData> InventoryEquippedSlotItems;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> InventoryConsumableSlotItems;

	UPROPERTY()
	TObjectPtr<class UItemComponent> ItemComponent;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FItemUISlotData> EquippableItemsMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FItemUISlotData> ConsumableItemsMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FItemUISlotData> OtherItemsMap;
};
