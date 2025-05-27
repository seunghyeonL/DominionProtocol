// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "Interface/ConsumableItemInterface.h"
#include "AddMaxPotionConsumable.generated.h"

class ADomiCharacter;
class UItemComponent;

UCLASS()
class DOMINIONPROTOCOL_API AAddMaxPotionConsumable : public ABaseItem, public IConsumableItemInterface
{
	GENERATED_BODY()

public:
	AAddMaxPotionConsumable();
	
protected:
	virtual void Consume_Implementation(AActor* ConsumedActor) override;
	virtual FText GetConsumeMessage_Implementation() const override;
};
