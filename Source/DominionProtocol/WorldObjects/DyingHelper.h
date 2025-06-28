// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InteractableInterface.h"
#include "Interface/StoryDependentInterface.h"
#include "DyingHelper.generated.h"

class UActorStateComponent;
class AItemDropped;
class UDialogueManager;
class USphereComponent;
class ADomiCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateDialogueManager, UDialogueManager*);

UCLASS()
class DOMINIONPROTOCOL_API ADyingHelper : public ACharacter, public IInteractableInterface, public IStoryDependentInterface
{
	GENERATED_BODY()

public:
	ADyingHelper();

	//Getter
	FORCEINLINE UActorStateComponent* GetActorStateComponent() const { return ActorStateComponent; }
	
	void Die();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;

	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;

	UFUNCTION()
	void ApplyDieState(bool bIsDead);
	
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

private:
	void SetIsInteractable(bool bNewIsInteractable);


public:
	FOnCreateDialogueManager OnCreateDialogueManager;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UActorStateComponent> ActorStateComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> InteractRadiusSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemDropped> ItemDroppedClass;

private:
	FTimerHandle DieTimerHandle;
	
	UPROPERTY()
	ADomiCharacter* CachedCharacter;

	UPROPERTY(EditAnywhere)
	FString DialogueID;

	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;
};
