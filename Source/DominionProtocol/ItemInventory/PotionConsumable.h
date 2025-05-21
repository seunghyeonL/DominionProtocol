#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "Interface/ConsumableItemInterface.h"
#include "PotionConsumable.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API APotionConsumable : public ABaseItem, public IConsumableItemInterface
{
	GENERATED_BODY()
	
public:
	APotionConsumable();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Potion")
	float RecoveryAmount=20.f;

	//Consume
	virtual void Consume_Implementation(AActor* ConsumedItem)override;
	virtual FText GetConsumeMessage_Implementation() const override;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyPotion(ADomiCharacter* TargetCharacter);
};
