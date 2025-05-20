// Fill out your copyright notice in the Description page of Project Settings.


#include "LightWeightActor.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "DomiFramework/WorldActorLoader/WorldActorLoaderSubsystem.h"

// Sets default values
ALightWeightActor::ALightWeightActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	SetRootComponent(ArrowComponent);
}

void ALightWeightActor::BeginPlay()
{
	Super::BeginPlay();

	WorldActorLoader = GetWorld()->GetSubsystem<UWorldActorLoaderSubsystem>();
	check(WorldActorLoader);
}

#if WITH_EDITOR
void ALightWeightActor::OnConstruction(const FTransform& Transform)
{
	if (!UniqueID.IsValid())
	{
		UniqueID = FGuid::NewGuid();
		Modify();
	}
}
#endif

#if WITH_EDITOR
void ALightWeightActor::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	if (!bDuplicateForPIE)
	{
		UniqueID = FGuid::NewGuid();
		Modify();
	}
}
#endif