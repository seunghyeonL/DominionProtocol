// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewTarget.h"


// Sets default values
AViewTarget::AViewTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}


