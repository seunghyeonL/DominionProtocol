// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/DomiCharacter.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "Engine/TargetPoint.h"
#include "AI/ProtoBoss/ProtoBossEnemy.h"
#include "EngineUtils.h"
#include "WorldObjects/Crack.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"


void AProtoLevel1GameMode::StartPlay()
{
	Super::StartPlay();

}

void AProtoLevel1GameMode::StartBattle()
{
	Super::StartBattle();
	Debug::Print(TEXT("StartBattle"));

	if (!GameInstance)
	{
		return;
	}
}

void AProtoLevel1GameMode::EndBattle()
{
	Super::EndBattle();
	Debug::Print(TEXT("EndBattle"));
}

void AProtoLevel1GameMode::OnPlayerDeath()
{
	Super::OnPlayerDeath();
}
