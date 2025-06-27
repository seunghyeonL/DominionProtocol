// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/SkillObject/Boss3Rock.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GeometryCollection/GeometryCollectionComponent.h"


ABoss3Rock::ABoss3Rock()
{
	PrimaryActorTick.bCanEverTick = true;

	RockMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("RockMesh"));
	RockMesh->SetupAttachment(RootComponent);

	RockMesh->SetSimulatePhysics(false);
	RockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABoss3Rock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(RockMesh))
	{
		RockMesh->AddLocalRotation(RotationPerSecond * DeltaTime);
	}
}

void ABoss3Rock::DestroyProjectile()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Loc = GetActorLocation();
	const FRotator Rot = GetActorRotation();

	if (BreakFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			BreakFX,
			GetActorLocation(),
			GetActorRotation(),
			FVector(2.f)
		);
	}


	if (IsValid(RockMesh))
	{
		//RockMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RockMesh->SetSimulatePhysics(true);
		RockMesh->SetEnableGravity(true);

		RockMesh->AddImpulse(FVector(0, 0, -3000.f), NAME_None, true);
		//RockMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	SetActorTickEnabled(false);

	SetLifeSpan(BreakRemainTime);
}