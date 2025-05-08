#include "ItemInventory/ItemSpawnPoint.h"

AItemSpawnPoint::AItemSpawnPoint()
{
 	PrimaryActorTick.bCanEverTick = false;
}

void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnItem();
}

void AItemSpawnPoint::SpawnItem()
{
	TSubclassOf<ABaseItem> ItemToSpawn = nullptr;

	if (ItemToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(ItemToSpawn, GetActorLocation() + SpawnOffset, GetActorRotation(), SpawnParams);

		UE_LOG(LogTemp, Log, TEXT("%s is successfully spawned"), *ItemToSpawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No item to spawn."));
	}

}
