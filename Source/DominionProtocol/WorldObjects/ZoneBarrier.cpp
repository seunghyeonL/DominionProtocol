// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/ZoneBarrier.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/StatusComponent/StatusComponent.h"

AZoneBarrier::AZoneBarrier()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AZoneBarrier::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AZoneBarrier::OnOverlapEnd);


    Wall= CreateDefaultSubobject<UBoxComponent>(TEXT("Wall"));
    Wall->SetupAttachment(RootComponent);
    
    Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZoneBarrier::ActivateBarrier()
{
    Debug::Print(TEXT("Activate Barrier"));

    Wall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Wall->SetCollisionProfileName(TEXT("BlockAll"));
}

void AZoneBarrier::DeactivateBarrier()
{
    Debug::Print(TEXT("Deactivate Barrier"));

    if (IsValid(Wall))
    {
        Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AZoneBarrier::OnPlayerDied()
{
    if (CachedCharacter)
    {
        Debug::Print(TEXT("ZoneBarrier: Player died in ZoneBarrier. Deactivating Barrier."));
        DeactivateBarrier();
    }
}

void AZoneBarrier::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
    {
        Debug::Print(TEXT("NotPlayer"));
        return;
    }

    ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
    ensure(PlayerCharacter);
    CachedCharacter = PlayerCharacter;

    PlayerCharacter->AddInteractableActor(this);
    if (UStatusComponent* Status = PlayerCharacter->GetStatusComponent())
    {
        Status->OnDeath.AddUObject(this, &AZoneBarrier::OnPlayerDied);
    }
}

void AZoneBarrier::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (IsValid(OtherActor) && OtherActor == CachedCharacter)
    {
        ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
        ensure(PlayerCharacter);

        CachedCharacter = nullptr;
        PlayerCharacter->RemoveInteractableActor(this);
    }
}

void AZoneBarrier::Interact_Implementation(AActor* Interactor)
{
    OnPlayerEnterZoneDelegate.Broadcast();
}

FText AZoneBarrier::GetInteractMessage_Implementation() const
{
    return FText::FromString(TEXT("보스와 전투 시작"));
}
