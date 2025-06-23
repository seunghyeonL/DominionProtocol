// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Teleporter.generated.h"

class ATargetPoint;
class UArrowComponent;
class ADomiCharacter;
class USphereComponent;

UCLASS()
class DOMINIONPROTOCOL_API ATeleporter : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

// Functions
public:
	ATeleporter();

// Getter
	FORCEINLINE bool GetShouldOnSkyAtmosphere() const { return bShouldOnSkyAtmosphere; }
	FORCEINLINE bool GetShouldOffSkyAtmosphere() const { return bShouldOffSkyAtmosphere; }

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
	void FindAndLinkAnother();
	
// Variables
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> TeleportPointComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FString TeleporterName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FString LinkedTeleporterName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bShouldOnSkyAtmosphere;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bShouldOffSkyAtmosphere;

private:
	UPROPERTY()
	TObjectPtr<ADomiCharacter> CachedCharacter;

	UPROPERTY()
	TObjectPtr<ATeleporter> LinkedTeleporter;

	
};
