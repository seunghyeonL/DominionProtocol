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

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(ArrowComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ALightWeightActor::BeginPlay()
{
	Super::BeginPlay();

	WorldActorLoader = GetWorld()->GetSubsystem<UWorldActorLoaderSubsystem>();
	check(WorldActorLoader);
}

void ALightWeightActor::OnConstruction(const FTransform& Transform)
{
#if WITH_EDITOR
	if (!UniqueID.IsValid())
	{
		UniqueID = FGuid::NewGuid();
		Modify();
	}
#endif
}

void ALightWeightActor::OnPlayerRegionEnter()
{
	WorldActorLoader->SpawnActorToTargetTransform(LinkedActorClass, ActorCategory, UniqueID, GetActorLocation(), GetActorRotation());
	
}
