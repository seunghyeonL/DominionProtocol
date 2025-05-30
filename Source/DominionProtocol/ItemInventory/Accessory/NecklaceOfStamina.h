// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Interface/EquipEffectableItemInterface.h"
#include "NecklaceOfStamina.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API ANecklaceOfStamina : public ABaseItem, public IEquipEffectableItemInterface
{
	GENERATED_BODY()
	
public:
	ANecklaceOfStamina();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ring")
	float MaxStaminaBoostAmount = 5.f;

	//EquipEffect
	virtual void Equip_Implementation(AActor* EquipCharacter)override;

	virtual void UnEquip_Implementation(AActor* UnequipCharacter)override;

	virtual FText GetEquipMessage_Implementation()const override;

public:
	UFUNCTION(BlueprintCallable)
	void EquipNecklaceOfStamina(ADomiCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void UnEquipNecklaceOfStamina(ADomiCharacter* TargetCharacter);
};
