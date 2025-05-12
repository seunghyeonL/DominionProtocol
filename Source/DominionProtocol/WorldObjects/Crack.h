// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Crack.generated.h"

class ATargetPoint;
class ADomiCharacter;
class USphereComponent;

UCLASS()
class DOMINIONPROTOCOL_API ACrack : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

#pragma region Functions
	
public:
	ACrack();
	
	FORCEINLINE void SwitchActivate() { bIsActivate = !bIsActivate; }

	FORCEINLINE FVector GetRespawnTargetPointLocation() const;
	
protected:
	virtual void BeginPlay() override;
	
	//Interact
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;
	
	void MoveToLevel();
	
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

#pragma endregion

	
	
#pragma region Variables
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereCollisionComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UChildActorComponent* RespawnTargetPointComp;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText CrackName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 CrackIndex;

private:
	ATargetPoint* RespawnTargetPoint;
	
	UPROPERTY()
	ADomiCharacter* CachedCharacter;
	
	bool bIsActivate;
	
#pragma endregion
};
