// Fill out your copyright notice in the Description page of Project Settings.


#include "DropEssence.h"
#include "Components/SphereComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"

#include "Util/DebugHelper.h"

ADropEssence::ADropEssence()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(SceneRoot);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractRadiusSphereComponent");
	SphereComponent->SetupAttachment(SceneRoot);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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

	WorldInstanceSubsystem->SetFieldEssenceAmount(GameInstance->GetPlayerCurrentEssence());
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

	GameInstance->SetPlayerCurrentEssence(GameInstance->GetPlayerCurrentEssence() + WorldInstanceSubsystem->GetFieldEssenceAmount());
	//나중에 복구 UI 만들어지면 그걸로 변경
	Debug::Print(TEXT("Essence Restored!!"));
	
	Destroy();
}

FText ADropEssence::GetInteractMessage_Implementation() const
{
	return IInteractableInterface::GetInteractMessage_Implementation();
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
