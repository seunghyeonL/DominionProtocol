#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Util/GameTagList.h"
#include "PawnTagInterface.generated.h"

UINTERFACE()
class UPawnTagInterface : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API IPawnTagInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetPawnTag();
};
