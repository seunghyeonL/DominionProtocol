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

void AStoryTrigger::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStoryTrigger::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AStoryTrigger::OnOverlapEnd);

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

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	ensure(PlayerCharacter);
	CachedCharacter = PlayerCharacter;

	PlayerCharacter->AddInteractableActor(this);
}

void AStoryTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);

		CachedCharacter = nullptr;
		PlayerCharacter->RemoveInteractableActor(this);
	}
	else
	{
		Debug::Print(TEXT("AStoryTrigger::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}

void AStoryTrigger::Interact_Implementation(AActor* Interactor)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
	if (!PlayerCharacter) return;
	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		GI->AdvanceStoryState();
	}

	DialogueManager = NewObject<UDialogueManager>(this);
	OnCreateDialogueManager.Broadcast(DialogueManager);
	FVector CrackLocation = GetActorLocation();
	FRotator CrackRotation = GetActorRotation();
	if (DialogueManager->TryStartDialogueIfExists(GI->GetCurrentGameStoryState(), CrackLocation, CrackRotation))
	{
		Debug::Print(TEXT("AStoryTrigger: 대사 종료"));

		return;
	}
	else
	{
		Debug::Print(TEXT("AStoryTrigger: 대사 없음"));
	}

	PlayerCharacter->RemoveInteractableActor(this);
}

FText AStoryTrigger::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("확인하기"));
}