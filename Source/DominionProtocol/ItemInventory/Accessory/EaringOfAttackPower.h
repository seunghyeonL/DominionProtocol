#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Interface/EquipEffectableItemInterface.h"
#include "EaringOfAttackPower.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API AEaringOfAttackPower : public ABaseItem, public IEquipEffectableItemInterface
{
	GENERATED_BODY()

public:
	AEaringOfAttackPower();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Earing")
	float AttackPowerBoostAmount = 5.f;

	//EquipEffect
	virtual void Equip_Implementation(AActor* EquipCharacter)override;

	virtual void UnEquip_Implementation(AActor* UnequipCharacter)override;

	virtual FText GetEquipMessage_Implementation()const override;

public:
	UFUNCTION(BlueprintCallable)
	void EquipEaringOfAttackPower(ADomiCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void UnEquipEaringOfAttackPower(ADomiCharacter* TargetCharacter);
};
