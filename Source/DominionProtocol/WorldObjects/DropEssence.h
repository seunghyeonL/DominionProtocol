// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Player/Characters/DomiCharacter.h"
#include "DropEssence.generated.h"

class UNiagaraComponent;
class UWorldInstanceSubsystem;
class UDomiGameInstance;
class USphereComponent;

UCLASS()
class DOMINIONPROTOCOL_API ADropEssence : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

//Functions
public:
	ADropEssence();

protected:
	virtual void BeginPlay() override;
	
	virtual void Interact_Implementation(AActor* Interactor) override;

	virtual FText GetInteractMessage_Implementation() const override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

//Variables
protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraComponent> NiagaraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;

private:
	UPROPERTY()
	TObjectPtr<ADomiCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UDomiGameInstance> GameInstance;

	UPROPERTY()
	TObjectPtr<UWorldInstanceSubsystem> WorldInstanceSubsystem;
	
	int32 EssenceAmount;
};
