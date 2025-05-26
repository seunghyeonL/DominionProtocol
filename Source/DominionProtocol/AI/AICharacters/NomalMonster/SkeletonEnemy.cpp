// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/SkeletonEnemy.h"


ASkeletonEnemy::ASkeletonEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitializeStatusComponent();
	InitializeSkillComponent();
}