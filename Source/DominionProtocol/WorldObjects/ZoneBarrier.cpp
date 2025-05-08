// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/ZoneBarrier.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Kismet/GameplayStatics.h"

AZoneBarrier::AZoneBarrier()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 
}

void AZoneBarrier::ActivateBarrier()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AZoneBarrier::DeactivateBarrier()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZoneBarrier::OnPlayerOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        if (AProtoLevel1GameMode* GM = Cast<AProtoLevel1GameMode>(UGameplayStatics::GetGameMode(this)))
        {
            GM->StartBattle();
        }
    }
}