#include "WorldObjects/BossSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Util/DebugHelper.h"

ABossSpawner::ABossSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->OnStoryStateChanged.AddDynamic(this, &ABossSpawner::OnStoryStateUpdated);
	}
}

void ABossSpawner::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	
}

void ABossSpawner::SpawnBoss()
{
	/*
	if (bHasSpawned || !BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - bHasSpawned || !BossClass || !BossTag.IsValid()"));
		return;
	}*/
	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (GI->GetCurrentGameStoryState() != RequiredStoryState)
		{
			Debug::Print(TEXT("BossSpawner: Story state not matched. Skip spawn."));
			return;
		}
	}
	if (!BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - !BossClass || !BossTag.IsValid()"));
		return;
	}
	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (GI->GetIsBossDead(BossTag))
		{
			Debug::Print(TEXT("BossSpawner: Boss is already dead. Skip spawn."));
			return;
		}
	}

	AActor* SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossClass, GetActorLocation(),	GetActorRotation());

	if (SpawnedBoss)
	{
		//bHasSpawned = true;
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
		OnBossSpawned.Broadcast(BossTag, RelatedBossDoor);
		OnBossSpawnedToWidget.Broadcast(SpawnedBoss);
	}
}