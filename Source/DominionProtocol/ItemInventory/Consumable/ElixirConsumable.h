#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "Interface/ConsumableItemInterface.h"
#include "ElixirConsumable.generated.h"

class ADomiCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API AElixirConsumable : public ABaseItem, public IConsumableItemInterface
{
	GENERATED_BODY()
	
public:
	AElixirConsumable();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Elixir")
	TArray<FGameplayTag> RemoveableStatusEffects;

	//Consume
	virtual void Consume_Implementation(AActor* ConsumedItem)override;
	virtual FText GetConsumeMessage_Implementation() const override;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyElixir(ADomiCharacter* TargetCharacter);

};
