// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInstanceSubsystem.h"

void UWorldInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	WorldNames.Add(TEXT("Proto_Level1"));
	WorldNames.Add(TEXT("Proto_Level2"));
	WorldNames.Add(TEXT("TestCrackLevel1"));
	WorldNames.Add(TEXT("TestCrackLevel2"));
}

void UWorldInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
