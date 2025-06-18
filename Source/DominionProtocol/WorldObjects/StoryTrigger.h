#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/InteractableInterface.h"
#include "StoryTrigger.generated.h"

class UBoxComponent;
class ADomiCharacter;
class UDialogueManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateDialogueManager, UDialogueManager*);

UCLASS()
class DOMINIONPROTOCOL_API AStoryTrigger : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AStoryTrigger();

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
