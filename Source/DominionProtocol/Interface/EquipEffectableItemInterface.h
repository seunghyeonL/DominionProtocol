#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipEffectableItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEquipEffectableItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOMINIONPROTOCOL_API IEquipEffectableItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consume")
	void Equip(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consume")
	void UnEquip(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consume")
	FText GetEquipMessage() const;
};
