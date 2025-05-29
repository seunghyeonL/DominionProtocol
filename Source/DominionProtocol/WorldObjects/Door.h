// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

class UBoxComponent;
class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API ADoor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ADoor();

	UFUNCTION()
	virtual void MoveDoor(float Value);

	UFUNCTION()
	virtual void OpenDoor();

	UFUNCTION()
	virtual void CloseDoor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Interact
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

	UFUNCTION()
	void OnTimelineFinished();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollisionComp;

	FTimeline Timeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	bool bIsDoorClosed;

	UPROPERTY(EditAnywhere)
	float DoorRotateAngle;
 
private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
};
