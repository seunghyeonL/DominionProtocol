// Fill out your copyright notice in the Description page of Project Settings.


#include "DropEssence.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"

#include "Util/DebugHelper.h"

ADropEssence::ADropEssence()
	:	EssenceAmount(0)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(SceneRoot);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRadiusSphereComponent"));
	SphereComponent->SetupAttachment(SceneRoot);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADropEssence::SetIsInteractable(bool bNewIsInteractable)
{
	if (bNewIsInteractable)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADropEssence::OnOverlapBegin);
		SphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ADropEssence::OnOverlapEnd);
	}
	else
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereComponent->OnComponentBeginOverlap.Clear();
		SphereComponent->OnComponentEndOverlap.Clear();
	}
}

void ADropEssence::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADropEssence::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADropEssence::OnOverlapEnd);

	GameInstance = GetGameInstance<UDomiGameInstance>();
	if (IsValid(GameInstance))
	{
		WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	}
	check(GameInstance);
	check(WorldInstanceSubsystem);
}

void ADropEssence::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(GameInstance))
	{
		GameInstance = GetGameInstance<UDomiGameInstance>();
	}
	if (!IsValid(WorldInstanceSubsystem))
	{
		WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	}
	
	check(GameInstance);
	check(WorldInstanceSubsystem);

	
	const int32 RestoredEssence = GameInstance->GetPlayerCurrentEssence() + WorldInstanceSubsystem->GetDropEssenceAmount();
	GameInstance->SetPlayerCurrentEssence(RestoredEssence);
	WorldInstanceSubsystem->SetDropEssenceCache(nullptr);
	WorldInstanceSubsystem->SetIsDropEssenceExist(false);
	WorldInstanceSubsystem->SetDropEssenceAmount(0);
	WorldInstanceSubsystem->SetDropEssenceLocation(FVector::ZeroVector);
	
	//나중에 복구 UI 만들어지면 그걸로 변경
	OnDropEssenceRestored.Broadcast(RestoredEssence);
	Debug::Print(TEXT("Essence Restored!!"));
	
	Destroy();
}

FText ADropEssence::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("균열 정수 회복"));
}

void ADropEssence::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) && !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	}

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->AddInteractableActor(this);
	}
}

void ADropEssence::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && IsValid(PlayerCharacter) && OtherActor == PlayerCharacter)
	{
		PlayerCharacter->RemoveInteractableActor(this);
	}
}
