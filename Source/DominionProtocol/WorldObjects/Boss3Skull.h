// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Boss3Skull.generated.h"

class ADomiCharacter;
class USphereComponent;

UCLASS()
class DOMINIONPROTOCOL_API ABoss3Skull : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ABoss3Skull();

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

private:
	void OnShake();

	void SpawnBoss3();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SkullMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> AltarMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> InteractableCollisionSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BossTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BossClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShakeTimeDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxShakeTime;
	
private:
	UPROPERTY()
	TObjectPtr<ADomiCharacter> CachedCharacter;

	FRotator BaseRotation;
	float ShakeStartTime;
	
	float TimeCount;

	FTimerHandle ShakeTimerHandle;
};
