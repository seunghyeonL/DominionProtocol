// Fill out your copyright notice in the Description page of Project Settings.


#include "DyingHelper.h"
#include "ItemInventory/ItemDropped.h"
#include "WorldObjects/DialogueManager.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"

#include "Util/DebugHelper.h"


ADyingHelper::ADyingHelper()
	: DialogueID(TEXT("Helper"))
{
	PrimaryActorTick.bCanEverTick = false;

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	Hair->SetupAttachment(GetMesh());

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetMesh());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	GetMesh()->bReceivesDecals = false;
}

void ADyingHelper::BeginPlay()
{
	Super::BeginPlay();

	DialogueManager = NewObject<UDialogueManager>(this);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADyingHelper::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ADyingHelper::OnOverlapEnd);
}

void ADyingHelper::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//DialogueManager = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ADyingHelper::Interact_Implementation(AActor* Interactor)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
	if (!PlayerCharacter) return;


	if (DialogueManager)
	{
		DialogueManager = NewObject<UDialogueManager>(this);
		OnCreateDialogueManager.Broadcast(DialogueManager);
		DialogueManager->TryStartDialogueByID(DialogueID);
	}
}

FText ADyingHelper::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("대화하기"));
}

void ADyingHelper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("Not Player"));
		return;
	}

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	ensure(PlayerCharacter);
	CachedCharacter = PlayerCharacter;

	PlayerCharacter->AddInteractableActor(this);
}

void ADyingHelper::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);

		CachedCharacter = nullptr;
		PlayerCharacter->RemoveInteractableActor(this);
	}
}

void ADyingHelper::Die()
{
	// 아이템 드랍
	if (IsValid(ItemDroppedClass.Get()))
	{
		GetWorld()->SpawnActor<AItemDropped>(ItemDroppedClass.Get(), GetActorLocation(), GetActorRotation());
	}

	GetWorldTimerManager().SetTimer(
		DieTimerHandle,
		[this]()
		{
			Destroy();
		},
		3.f,
		false);
}
