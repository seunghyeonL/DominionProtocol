#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Interface/EquipEffectableItemInterface.h"
#include "RingOfHealth.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API ARingOfHealth : public ABaseItem, public IEquipEffectableItemInterface
{
	GENERATED_BODY()
	
public:
	ARingOfHealth();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ring")
	float MaxHealthBoostAmount = 5.f;
	
	//EquipEffect
	virtual void Equip_Implementation(AActor* EquipCharacter)override;
	virtual FText GetEquipMessage_Implementation()const override;

public:
	UFUNCTION(BlueprintCallable)
	void EquipRingOfHealth(ADomiCharacter* TargetCharacter);
};
