// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableActorBase.h"

APoolableActorBase::APoolableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APoolableActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (UPrimitiveComponent* PrimitiveComponent = FindComponentByClass<UPrimitiveComponent>())
	{
		PrimitiveComponent->bReceivesDecals = false;
	}
}

void APoolableActorBase::OnObjectSpawn_Implementation()
{
	bIsActivate = true;
	SetActorHiddenInGame(!bIsActivate);
	SetActorEnableCollision(bIsActivate);
}

void APoolableActorBase::OnObjectReset_Implementation()
{
	bIsActivate = false;
	SetActorHiddenInGame(!bIsActivate);
	SetActorEnableCollision(bIsActivate);
}
