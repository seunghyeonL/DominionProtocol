// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIUtil/PatrolPoint.h"

APatrolPoint::APatrolPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;
}