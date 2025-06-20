#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/InteractableInterface.h"
#include "Interface/StoryDependentInterface.h"
#include "StoryTrigger.generated.h"

class UBoxComponent;
class ADomiCharacter;
class UDialogueManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateDialogueManager, UDialogueManager*);

UCLASS()
class DOMINIONPROTOCOL_API AStoryTrigger : public AActor, public IInteractableInterface, public IStoryDependentInterface
{
	GENERATED_BODY()
	
public:	
	AStoryTrigger();

	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;
	FOnCreateDialogueManager OnCreateDialogueManager;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameStoryState ForcedStoryState;

	UPROPERTY()
	UDialogueManager* DialogueManager;
private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
};
