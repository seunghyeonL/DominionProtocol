#include "WorldObjects/BossTrigger.h"
#include "Components/BoxComponent.h"
#include "WorldObjects/BossSpawner.h"

ABossTrigger::ABossTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);
}

void ABossTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossTrigger::OnTriggerOverlapBegin);
}

void ABossTrigger::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;

	if (IsValid(LinkedBossSpawner))
	{
		LinkedBossSpawner->SpawnBoss();
	}
}
