// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "DomiFramework/GameMode/ProtoLevel2GameMode.h"
#include "Util/DebugHelper.h"

// Sets default values
ACrack::ACrack()
	: bIsActivate(true)
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetGenerateOverlapEvents(true);
	Collision->SetupAttachment(Scene);
    
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
}

void ACrack::MoveToLevel()
{

}

void ACrack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    if (!OtherActor || !OtherActor->ActorHasTag("Player"))
    {
        return;
    }

    FName TargetLevelName = NAME_None;

    AGameModeBase* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        if (GameMode->IsA(AProtoLevel1GameMode::StaticClass()))
        {
            TargetLevelName = FName("Proto_Level2"); 
        }
        else if (GameMode->IsA(AProtoLevel2GameMode::StaticClass()))
        {
            TargetLevelName = FName("Proto_Level1"); 

        }
    }

    if (TargetLevelName != NAME_None)
    {
        Debug::Print(FString::Printf(TEXT("Open %s"), *TargetLevelName.ToString()));
        UGameplayStatics::OpenLevel(this, TargetLevelName);

    }
}
