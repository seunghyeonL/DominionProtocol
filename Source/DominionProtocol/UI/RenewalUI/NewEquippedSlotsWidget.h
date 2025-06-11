// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewEquippedSlotsWidget.generated.h"


struct FItemUISlotData;

UCLASS()
class DOMINIONPROTOCOL_API UNewEquippedSlotsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnUpdateEquippableSlotItems();
	UFUNCTION()
	void OnUpdateConsumableSlotItems();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquippableSlotItems();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateConsumableSlotItems();
	
protected:
	virtual void NativeConstruct() override;
	
	void BindItemSlotsDelegate();


protected:
	UPROPERTY()
	TObjectPtr<class UItemComponent> ItemComponent;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FItemUISlotData> EquippableSlotMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FItemUISlotData> ConsumableSlotMap;
	
};
