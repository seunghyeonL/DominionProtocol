// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "Interface/ConsumableItemInterface.h"
#include "StaminaConsumable.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API AStaminaConsumable : public ABaseItem, public IConsumableItemInterface
{
	GENERATED_BODY()
	
public:
	AStaminaConsumable();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Potion")
	float RecoveryAmount = 50.f;

	//Consume
	virtual void Consume_Implementation(AActor* ConsumedActor)override;
	virtual FText GetConsumeMessage_Implementation() const override;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyStaminaPotion(ADomiCharacter* TargetCharacter);
};
