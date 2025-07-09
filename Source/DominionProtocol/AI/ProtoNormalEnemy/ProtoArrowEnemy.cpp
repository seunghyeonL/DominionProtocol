// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoArrowEnemy.h"

#include "Util/DebugHelper.h"


// Sets default values
AProtoArrowEnemy::AProtoArrowEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnTag = PawnTags::ArrowProto;
}

// Called when the game starts or when spawned
void AProtoArrowEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProtoArrowEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AProtoArrowEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
