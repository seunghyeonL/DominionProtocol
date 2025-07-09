// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RenewalUI/NewMenuWidget.h"
#include "NewEquipmentWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UNewEquipmentWidget : public UNewMenuWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	void BindEquippedSlotsDelegates();

	UFUNCTION()
	void OnUpdateEquippedSlots();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquippedSlots();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UItemComponent> ItemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UStatusComponent> StatusComponent;
};
