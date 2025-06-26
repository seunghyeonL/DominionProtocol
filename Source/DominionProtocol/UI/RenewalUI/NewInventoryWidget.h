// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewInventoryWidget.generated.h"

struct FItemUISlotData;


UENUM(BlueprintType)
enum class EDisplayArray : uint8
{
	AllItemsArray				UMETA(DisplayName="AllItemsArray"),
	WeaponItemsArray			UMETA(DisplayName="WeaponItemsArray"),
	AccessoryItemsArray			UMETA(DisplayName="AccessoryItemsArray"),
	ConsumableItemsArray		UMETA(DisplayName="ConsumableItemsArray"),
	SkillItemsArray				UMETA(DisplayName="SkillItemsArray"),
	OtherItemsArray				UMETA(DisplayName="OtherItemsArray"),
};


UCLASS()
class DOMINIONPROTOCOL_API UNewInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void OnUpdateInventoryItemList();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventoryItemList();
	
protected:
	virtual void NativeConstruct() override;
	
	void BindInventoryDelegates();
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UItemComponent> ItemComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> InventoryAllItems;
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> WeaponItemsItems;
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> AccessoryItemsItems;
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> SkillItemsItems;
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> ConsumableItemsArray;
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemUISlotData> OtherItemsArray;
	
	UPROPERTY(BlueprintReadWrite)
	EDisplayArray LastDisplayArray = EDisplayArray::AllItemsArray;
	
};
