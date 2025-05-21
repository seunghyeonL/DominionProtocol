#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConsumableItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UConsumableItemInterface : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API IConsumableItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consume")
	void Consume(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consume")
	FText GetConsumeMessage() const;
};
