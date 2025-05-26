// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Components/TimelineComponent.h"
#include "Elevator.generated.h"

class UBoxComponent;
class ADomiCharacter;
class AElevatorCaller;

UCLASS()
class DOMINIONPROTOCOL_API AElevator : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AElevator();

	void MoveElevatorTo(float TargetZ);

	bool IsMoving() const { return bIsMoving; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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

	// void CallElevator();
	UFUNCTION()
	void ActivateElevator(float Value);

	UFUNCTION()
	void OnTimelineFinished();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elevator")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollisionComp;

	FTimeline Timeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere)
	float MoveDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	bool bIsMoving;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Elevator")
	AElevatorCaller* TopCaller;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Elevator")
	AElevatorCaller* BottomCaller;


private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;

	FVector StartLocation;
	FVector TargetLocation;

	bool bIsAtTop;
};
