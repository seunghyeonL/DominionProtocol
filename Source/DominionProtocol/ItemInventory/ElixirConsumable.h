#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "ElixirConsumable.generated.h"

class ADominionProtocolCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API AElixirConsumable : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AElixirConsumable();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Elixir")
	TArray<FGameplayTag> RemoveableStatusEffects;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyElixir(ADominionProtocolCharacter* TargetCharacter);

};
