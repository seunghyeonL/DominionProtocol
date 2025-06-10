#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Interface/EquipEffectableItemInterface.h"
#include "Util/GameTagList.h"
#include "AccessoryOfImmunity.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API AAccessoryOfImmunity : public ABaseItem, public IEquipEffectableItemInterface
{
	GENERATED_BODY()
	
public:
	AAccessoryOfImmunity();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Accessory")
	FGameplayTag ImmuneStatusEffectTag;

	// IEquipEffectableItemInterface 
	virtual void Equip_Implementation(AActor* EquipCharacter) override;
	virtual void UnEquip_Implementation(AActor* UnequipCharacter) override;
	virtual FText GetEquipMessage_Implementation() const override;

public:
	UFUNCTION(BlueprintCallable, Category = "Accessory")
	void EquipAccessoryOfImmunity(ADomiCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Accessory")
	void UnEquipAccessoryOfImmunity(ADomiCharacter* TargetCharacter);
};
