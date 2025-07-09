#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/StoryDependentInterface.h"
#include "StoryTrigger.generated.h"

class UBoxComponent;
class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API AStoryTrigger : public AActor, public IStoryDependentInterface
{
	GENERATED_BODY()
	
public:	
	AStoryTrigger();

	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameStoryState ForcedStoryState;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
};
