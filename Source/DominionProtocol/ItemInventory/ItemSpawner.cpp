#include "ItemInventory/ItemSpawner.h"
#include "ItemSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"

AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(SceneRoot);
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnPoint::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AItemSpawnPoint* SpawnPoint = Cast<AItemSpawnPoint>(Actor);
		if (SpawnPoint)
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}
	SpawnItems();
}

void AItemSpawner::SpawnItems()
{
	if (ItemClasses.Num() == 0 || SpawnPoints.Num() == 0)
	{
		Debug::Print(TEXT("[ItemSpawner] class is none "));
		return;
	}

	for (AItemSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnedItems.Contains(SpawnPoint) && SpawnedItems[SpawnPoint] != nullptr)
		{
			continue;
		}
		int32 RandomIndex = FMath::RandRange(0, ItemClasses.Num() - 1);
		TSubclassOf<ABaseItem> RandomItemClass = ItemClasses[RandomIndex];

		if (RandomItemClass)
		{
			ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(RandomItemClass, SpawnPoint->GetActorLocation(), FRotator::ZeroRotator);
			if (SpawnedItem)
			{
				SpawnedItem->OnDestroyed.AddDynamic(this, &AItemSpawner::OnItemDestroyed);

				SpawnedItems.Add(SpawnPoint, SpawnedItem);
			}
		}
	}
}

void AItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor)
	{
		return;
	}
	ABaseItem* DestroyedItem = Cast<ABaseItem>(DestroyedActor);
	if (!DestroyedActor)
	{
		return;
	}
	Debug::Print(TEXT("[ItemSpawner] class is deleted "));

	for (auto& Elem : SpawnedItems)
	{
		if (Elem.Value == DestroyedItem)
		{
			AItemSpawnPoint* SpawnPoint = Elem.Key;
			SpawnedItems[SpawnPoint] = nullptr;

			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateLambda([this, SpawnPoint]()
				{
					if (SpawnPoint)
					{
						this->SpawnItems();
					}
				}), RespawnTime, false);

			break;
		}
	}
}

void AItemSpawner::RespawnItem(AItemSpawnPoint* SpawnPoint)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateUObject(this, &AItemSpawner::SpawnItems),
		RespawnTime,
		false
	);
}