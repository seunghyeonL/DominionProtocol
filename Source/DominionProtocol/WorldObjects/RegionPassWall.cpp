// Fill out your copyright notice in the Description page of Project Settings.


#include "RegionPassWall.h"


// Sets default values
ARegionPassWall::ARegionPassWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARegionPassWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARegionPassWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

