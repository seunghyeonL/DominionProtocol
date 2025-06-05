#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/InteractableInterface.h"
#include "StoryTrigger.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AStoryTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AStoryTrigger();

protected:
	virtual void BeginPlay() override;

};
