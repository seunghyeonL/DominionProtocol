// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss2Enemy.h"
#include "Components/StatusComponent/StatusComponent.h"

// Sets default values
ABoss2Enemy::ABoss2Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BossID = 2;
	PawnTag = PawnTags::Boss2;
}

// Called when the game starts or when spawned
void ABoss2Enemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoss2Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss2Enemy::OnParried_Implementation()
{
	StatusComponent->ActivateStatusEffect(EffectTags::Parried, 0.f, 3.f);
}

// Called to bind functionality to input
void ABoss2Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}