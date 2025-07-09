#include "WorldObjects/StoryTrigger.h"
#include "Components/BoxComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"
#include "Player/Characters/DomiCharacter.h"
#include "WorldObjects/DialogueManager.h"

AStoryTrigger::AStoryTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AStoryTrigger::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->GetCurrentGameStoryState()>=ForcedStoryState)
	{
		if (CollisionBox)
		{
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AStoryTrigger::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStoryTrigger::OnOverlapBegin);

}

void AStoryTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("AStoryTrigger: Overlap"));

	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("Not Player"));
		return;
	}

	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI || GI->GetCurrentGameStoryState() >= ForcedStoryState)
	{
		Debug::Print(TEXT("Not RequiredStoryState"));
		return;
	}
	GI->SetCurrentGameStoryState(ForcedStoryState);
}
