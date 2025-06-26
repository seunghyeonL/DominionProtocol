// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "WorldObjects/DialogueManager.h"
#include "UI/UIInGame/NewDialogueWidget.h"

ABoss4Enemy::ABoss4Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	BossID = 4;
	PawnTag = PawnTags::Boss4;
	DialogueID = TEXT("Witch");

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABoss4Enemy::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABoss4Enemy::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABoss4Enemy::OnOverlapEnd);

	if (!DialogueManager)
	{
		DialogueManager = NewObject<UDialogueManager>(this);
	}
}

void ABoss4Enemy::Interact_Implementation(AActor* Interactor)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
	if (!PlayerCharacter) return;

	if (!DialogueManager)
	{
		DialogueManager = NewObject<UDialogueManager>(this);
		UE_LOG(LogTemp, Display, TEXT("새로만듦"));
	}
	DialogueManager->TryStartDialogueByID(DialogueID);
}

FText ABoss4Enemy::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("확인하기"));
}

void ABoss4Enemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("ABoss4Enemy::OnOverlapBegin"));
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	ensure(PlayerCharacter);
	CachedCharacter = PlayerCharacter;

	PlayerCharacter->AddInteractableActor(this);
}

void ABoss4Enemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);

		CachedCharacter = nullptr;
		PlayerCharacter->RemoveInteractableActor(this);
	}
}