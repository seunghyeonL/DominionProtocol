// Fill out your copyright notice in the Description page of Project Settings.


#include "DyingHelper.h"
#include "ItemInventory/ItemDropped.h"
#include "WorldObjects/DialogueManager.h"
#include "Components/SphereComponent.h"
#include "Player/Characters/DomiCharacter.h"

#include "Util/DebugHelper.h"


ADyingHelper::ADyingHelper()
	: DialogueID(TEXT("Helper"))
{
	PrimaryActorTick.bCanEverTick = false;

	ActorStateComponent = CreateDefaultSubobject<UActorStateComponent>(TEXT("ActorStateComponent"));
	ActorStateComponent->SetGameplayTag(WorldActorTags::DyingHelper);
	
	InteractRadiusSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRadiusSphereCollision"));
	InteractRadiusSphereComponent->SetupAttachment(GetMesh());
	InteractRadiusSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractRadiusSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractRadiusSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	GetMesh()->bReceivesDecals = false;
}

void ADyingHelper::BeginPlay()
{
	Super::BeginPlay();

	ActorStateComponent->OnDyingHelperDead.AddDynamic(this, &ADyingHelper::ApplyDieState);
	
	if (ActorStateComponent->GetActorData().bIsDead)
	{
		ApplyDieState(true);
	}
	
	DialogueManager = NewObject<UDialogueManager>(this);

	InteractRadiusSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADyingHelper::OnOverlapBegin);
	InteractRadiusSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADyingHelper::OnOverlapEnd);

	UDomiGameInstance* GameInstance = GetWorld()->GetGameInstance<UDomiGameInstance>();
	GameInstance->OnStoryStateChanged.AddDynamic(this, &ADyingHelper::OnStoryStateUpdated);
	SetIsInteractable(false);
}

void ADyingHelper::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ActorStateComponent->OnDyingHelperDead.Clear();
	UDomiGameInstance* GameInstance = GetWorld()->GetGameInstance<UDomiGameInstance>();
	GameInstance->OnStoryStateChanged.Clear();

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
		DialogueManager->TryStartDialogueByID(DialogueID, this);
	}
}

FText ADyingHelper::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("대화하기"));
}

void ADyingHelper::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	if (ActorStateComponent->GetActorData().bIsDead)
	{
		return;
	}
	
	if (NewState == EGameStoryState::Clear_Boss3)
	{
		SetIsInteractable(true);
	}
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

void ADyingHelper::ApplyDieState(bool bIsDead)
{
	SetIsInteractable(false);
	GetMesh()->Stop();
}

void ADyingHelper::SetIsInteractable(bool bNewIsInteractable)
{
	if (bNewIsInteractable)
	{
		InteractRadiusSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractRadiusSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractRadiusSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		InteractRadiusSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADyingHelper::OnOverlapBegin);
		InteractRadiusSphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ADyingHelper::OnOverlapEnd);
	}
	else
	{
		InteractRadiusSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractRadiusSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractRadiusSphereComponent->OnComponentBeginOverlap.Clear();
		InteractRadiusSphereComponent->OnComponentEndOverlap.Clear();
	}
}

void ADyingHelper::Die()
{
	ActorStateComponent->SwitchStateAndUpdateInstance(WorldActorTags::DyingHelper);

	ApplyDieState(true);
	
	// 아이템 드랍
	if (IsValid(ItemDroppedClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AItemDropped>(ItemDroppedClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}
