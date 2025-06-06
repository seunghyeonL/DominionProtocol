#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/InteractableInterface.h"
#include "StoryTrigger.generated.h"

class UBoxComponent;
class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API AStoryTrigger : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AStoryTrigger();

protected:
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	EGameStoryState ForcedStoryState;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
};
