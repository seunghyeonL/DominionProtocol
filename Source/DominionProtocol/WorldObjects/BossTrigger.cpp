#include "WorldObjects/BossTrigger.h"
#include "Components/BoxComponent.h"
#include "WorldObjects/BossSpawner.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Util/DebugHelper.h"

ABossTrigger::ABossTrigger()
	: bHasStoryUpdated(false)
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
	Debug::Print(TEXT("BossTrigger: OnTriggerOverlapBegin Called"));

	if (!IsValid(OtherActor))
	{
		Debug::Print(TEXT("BossTrigger: OtherActor is not valid"));
		return;
	}

	Debug::Print(FString::Printf(TEXT("BossTrigger: Overlapped Actor = %s"), *OtherActor->GetName()));

	if (!OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("BossTrigger: Actor has no 'Player' tag"));
		return;
	}

	if (!IsValid(LinkedBossSpawner))
	{
		Debug::Print(TEXT("BossTrigger: LinkedBossSpawner is null"));
		return;
	}

	if (!bHasStoryUpdated)
	{
		Debug::Print(TEXT("BossTrigger: 스토리 상태 변경"));
		bHasStoryUpdated = true;
		if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			GI->SetCurrentGameStoryState(StoryStateToSet);
			// GI->AdvanceStoryState();
		}
	}

	Debug::Print(TEXT("BossTrigger: Calling SpawnBoss()"));
	LinkedBossSpawner->SpawnBoss();

}