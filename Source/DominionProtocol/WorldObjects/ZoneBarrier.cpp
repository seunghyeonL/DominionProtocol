// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldObjects/ZoneBarrier.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"

AZoneBarrier::AZoneBarrier()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AZoneBarrier::OnPlayerOverlap);
    
    Wall_Left = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall_Left"));
    Wall_Left->SetupAttachment(RootComponent);
    Wall_Right = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall_Right"));
    Wall_Right->SetupAttachment(RootComponent);
    Wall_Front = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall_Front"));
    Wall_Front->SetupAttachment(RootComponent);
    Wall_Back = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall_Back"));
    Wall_Back->SetupAttachment(RootComponent);
    

    TArray<UBoxComponent*> Walls = { Wall_Left, Wall_Right, Wall_Front, Wall_Back };
    for (UBoxComponent* Wall : Walls)
    {
        Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AZoneBarrier::ActivateBarrier()
{
    TArray<UBoxComponent*> Walls = { Wall_Left, Wall_Right, Wall_Front, Wall_Back };
    for (UBoxComponent* Wall : Walls)
    {
        Wall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Wall->SetCollisionProfileName(TEXT("BlockAll"));
    }
}

void AZoneBarrier::DeactivateBarrier()
{
    TArray<UBoxComponent*> Walls = { Wall_Left, Wall_Right, Wall_Front, Wall_Back };
    for (UBoxComponent* Wall : Walls)
    {
        Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
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