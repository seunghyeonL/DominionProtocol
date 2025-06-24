// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/StoryDependentInterface.h"
#include "Interface/InteractableInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h" 
#include "DominionProtocol/Util/GameTagList.h"
#include "WorldObjects/DialogueManager.h"
#include "BlockedPath.generated.h"

class UBoxComponent;
class ADomiCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateDialogueManager, UDialogueManager*);

UCLASS()
class DOMINIONPROTOCOL_API ABlockedPath : public AActor, public IStoryDependentInterface, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ABlockedPath();

	FOnCreateDialogueManager OnCreateDialogueManager;

	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TryOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpened();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BlockingBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* PathEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraTemplate;

	UPROPERTY(EditAnywhere, Category = "BlockedPath")
	bool bIsBlocking;

	UPROPERTY(EditAnywhere, Category = "BlockedPath")
	EGameStoryState RequiredStoryState = EGameStoryState::Tutorial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlockedPath")
	FGameplayTag RequiredKey;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;

	UPROPERTY(EditAnywhere)
	FString DialogueID;

	UPROPERTY()
	UDialogueManager* DialogueManager;
};
