// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Components/TimelineComponent.h"
#include "Lever.generated.h"

class UBoxComponent;
class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API ALever : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ALever();

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

	UFUNCTION()
	virtual void HandleLever(float Value);

	UFUNCTION()
	void OnTimelineFinished();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lever")
	UStaticMeshComponent* LeverMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lever")
	UStaticMeshComponent* HandleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollisionComp;

	FTimeline Timeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	bool bIsOn;

	UPROPERTY(EditAnywhere)
	float LeverRotateAngle;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;

	FRotator InitialRotation;
};
