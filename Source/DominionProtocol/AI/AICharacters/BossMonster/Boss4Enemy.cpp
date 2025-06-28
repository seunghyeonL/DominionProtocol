// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/DomiCharacter.h"
#include "WorldObjects/DialogueManager.h"
#include "UI/UIInGame/NewDialogueWidget.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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

void ABoss4Enemy::DropItem()
{
	// 일반 DroppedItem 로직
	if (IsValid(DropItemClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AItemDropped>(DropItemClass, GetGroundSpawnLocation(), GetActorRotation(), SpawnParams);
	}

	// 스토리 필수 아이템 자동 습득
	if (StoryItemTag.IsValid())
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
		if (IsValid(PlayerCharacter))
		{
			UItemComponent* ItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
			if (IsValid(ItemComponent))
			{
				ItemComponent->AddItem(StoryItemTag, 1);
			}
		}
	}
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

void ABoss4Enemy::StartFade()
{

	FadeElapsedTime = 0.0f;

	if (FadeMPC)
	{
		MPCInstance = GetWorld()->GetParameterCollectionInstance(FadeMPC);
	}

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&ABoss4Enemy::UpdateFade,
		0.05f,
		true
	);
}



void ABoss4Enemy::UpdateFade()
{
	if (!FadeCurve || !MPCInstance) return;

	FadeElapsedTime += 0.05f;
	float Alpha = FadeElapsedTime / FadeDuration;
	float Value = FadeCurve->GetFloatValue(Alpha * FadeDuration);

	MPCInstance->SetScalarParameterValue("FadeRadius", Value);

	if (FadeElapsedTime >= FadeDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}
}