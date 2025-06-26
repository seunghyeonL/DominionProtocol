#include "WorldObjects/BossSpawner.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Util/DebugHelper.h"

ABossSpawner::ABossSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossSpawner::BeginPlay()
{
	Super::BeginPlay();
}


void ABossSpawner::SpawnBoss()
{
	if (!BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - !BossClass || !BossTag.IsValid()"));
		return;
	}
	AActor* SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossClass, GetActorLocation(),	GetActorRotation());
	if (BossClass->IsChildOf(ABoss4Enemy::StaticClass()))
	{
		if (ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(SpawnedBoss))
		{
			if (UDialogueManager* DialogueManager = Boss4->GetDialogueManager())
			{
				OnCreateDialogueManager.Broadcast(DialogueManager);
			}
		}
	}

	if (SpawnedBoss)
	{
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
		if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->StartBattle(SpawnedBoss);
		}
	}
}