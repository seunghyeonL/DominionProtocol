// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiBaseAIController.h"


// Sets default values
ADomiBaseAIController::ADomiBaseAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADomiBaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADomiBaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

