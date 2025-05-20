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

	//Setter

	//Getter
	FORCEINLINE EWorldActorCategory GetActorCategory() const { return ActorCategory; }

	FORCEINLINE FGuid GetUniqueGuid() const { return UniqueID; }

	FORCEINLINE TSubclassOf<AActor> GetLinkedActorClass() const { return LinkedActorClass; }

	FORCEINLINE int32 GetLocatedRegionIndex() const { return LocatedRegionIndex; }

protected:
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
#endif

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<AActor> LinkedActorClass;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	EWorldActorCategory ActorCategory;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 LocatedRegionIndex;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	FGuid UniqueID;

	UPROPERTY()
	UWorldActorLoaderSubsystem* WorldActorLoader;
};
