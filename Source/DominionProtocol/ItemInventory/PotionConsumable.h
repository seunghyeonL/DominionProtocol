#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "PotionConsumable.generated.h"

class ADominionProtocolCharacter;
class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API APotionConsumable : public ABaseItem
{
	GENERATED_BODY()
	
public:
	APotionConsumable();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Potion")
	float RecoveryAmount=20.f;


public:
	UFUNCTION(BlueprintCallable)
	void ApplyPotion(ADominionProtocolCharacter* TargetCharacter);
};
