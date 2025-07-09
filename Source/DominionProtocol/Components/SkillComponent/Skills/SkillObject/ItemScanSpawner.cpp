#include "Components/SkillComponent/Skills/SkillObject/ItemScanSpawner.h"

AItemScanSpawner::AItemScanSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItemScanSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemScanSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 블루프린트에서 구현
}

