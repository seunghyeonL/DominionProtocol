#include "ItemInventory/BreakableChestItem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABreakableChestItem::ABreakableChestItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABreakableChestItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableChestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

