// Fill out your copyright notice in the Description page of Project Settings.


#include "Teleporter.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Player/Characters/DomiCharacter.h"

#include "Util/DebugHelper.h"


ATeleporter::ATeleporter()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComp);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(SceneComp);
}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATeleporter::OnOverlapEnd);
}

void ATeleporter::Interact_Implementation(AActor* Interactor)
{
	if (IsValid(CachedCharacter))
	{
		CachedCharacter->TeleportTo(LinkedTeleporter->GetActorLocation(), LinkedTeleporter->GetActorRotation());
	}
	else
	{
		Debug::Print(TEXT("ATeleporter::Interact : CachedCharacter is invalid"));
	}
}

FText ATeleporter::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("순간이동"));
}

void ATeleporter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	if (!IsValid(CachedCharacter))
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);
		CachedCharacter = PlayerCharacter;
	}

	CachedCharacter->AddInteractableActor(this);
}

void ATeleporter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		if (!IsValid(CachedCharacter))
		{
			ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
			ensure(PlayerCharacter);
			CachedCharacter = PlayerCharacter;
		}
		
		CachedCharacter->RemoveInteractableActor(this);
	}
}
