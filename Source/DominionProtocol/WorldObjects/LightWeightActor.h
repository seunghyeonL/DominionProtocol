// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/ECategory.h"
#include "LightWeightActor.generated.h"

class UWorldActorLoaderSubsystem;
class ADomiCharacter;
class USphereComponent;
class UArrowComponent;

UCLASS()
class DOMINIONPROTOCOL_API ALightWeightActor : public AActor
{
	GENERATED_BODY()

public:
	ALightWeightActor();

	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
	void OnPlayerRegionEnter();
	
protected:
	// Components begin
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<AActor> LinkedActorClass;
	// Components end


	// Variables
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	EWorldActorCategory ActorCategory;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 LocatedRegion;
	
	UPROPERTY()
	FGuid UniqueID;

	UPROPERTY()
	ADomiCharacter* PlayerCharacter;

	UPROPERTY()
	UWorldActorLoaderSubsystem* WorldActorLoader;
};
