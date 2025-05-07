// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiBaseEnemy.h"


// Sets default values
ADomiBaseEnemy::ADomiBaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADomiBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADomiBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADomiBaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

