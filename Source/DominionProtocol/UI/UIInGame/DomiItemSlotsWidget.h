// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiItemSlotsWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiItemSlotsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnSwapWeapons();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDisplayEquipWeapons();


protected:
	virtual void NativeConstruct() override;

	void BindSwapWeaponsEvent();

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bActiveButton = false;
};
