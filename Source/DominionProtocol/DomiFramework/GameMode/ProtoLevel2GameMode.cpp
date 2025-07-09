// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoLevel2GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "WorldObjects/Crack.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "EngineUtils.h"
#include "Util/DebugHelper.h"

AProtoLevel2GameMode::AProtoLevel2GameMode()
{
}

void AProtoLevel2GameMode::StartPlay()
{
	Super::StartPlay();
}

void AProtoLevel2GameMode::BeginPlay()
{
	Super::BeginPlay();
}