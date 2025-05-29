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
#include "WorldObjects/BossSpawner.h"


void AProtoLevel1GameMode::StartPlay()
{
	Super::StartPlay();

	for (TActorIterator<ABossSpawner> It(GetWorld()); It; ++It)
	{
		It->OnBossSpawned.AddDynamic(this, &AProtoLevel1GameMode::StartBattle);
	}
}

void AProtoLevel1GameMode::StartBattle(FGameplayTag BossTag, ABossRoomDoor* BossDoor)
{
	Super::StartBattle(BossTag, BossDoor);
	Debug::Print(FString::Printf(TEXT("StartBattle: Boss [%s]"), *BossTag.ToString()));

	if (!GameInstance)
	{
		return;
	}

	if (IsValid(BossDoor))
	{
		BossDoor->CloseDoor();
	}
}

void AProtoLevel1GameMode::EndBattle()
{
	Super::EndBattle();
	Debug::Print(TEXT("EndBattle"));

	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->AdvanceStoryState();
	}
}

void AProtoLevel1GameMode::OnPlayerDeath()
{
	Super::OnPlayerDeath();
}
