// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss1Enemy.h"

#include "Components/StatusComponent/StatusComponent.h"

// Sets default values
ABoss1Enemy::ABoss1Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BossID = 1;
	PawnTag = PawnTags::Boss1;
}

// Called when the game starts or when spawned
void ABoss1Enemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoss1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoss1Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoss1Enemy::OnGroggy()
{
	// Super::OnGroggy();
	StatusComponent->ActivateStatusEffect(EffectTags::Groggy, 0.f, 8.f);
}
