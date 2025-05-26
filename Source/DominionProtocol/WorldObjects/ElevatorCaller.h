// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "ElevatorCaller.generated.h"

class UBoxComponent;
class ADomiCharacter;
class AElevator;

UCLASS()
class DOMINIONPROTOCOL_API AElevatorCaller : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AElevatorCaller();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elevator")
	UStaticMeshComponent* TriggerMesh; 

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Elevator")
	AElevator* Elevator;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
};
