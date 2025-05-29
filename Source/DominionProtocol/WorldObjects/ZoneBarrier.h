// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Components/BoxComponent.h"
#include "ZoneBarrier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterZone);

class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API AZoneBarrier : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AZoneBarrier();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerEnterZone OnPlayerEnterZoneDelegate;

	void ActivateBarrier();
	void DeactivateBarrier();

	void OnPlayerDied();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
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

	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;


protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;

	UBoxComponent* Wall;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
};
