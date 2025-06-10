#include "WorldObjects/BossSpawner.h"
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

	if (SpawnedBoss)
	{
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
		OnBossSpawnedToWidget.Broadcast(SpawnedBoss);
	}
}