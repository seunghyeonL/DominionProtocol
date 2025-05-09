// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "DomiFramework/GameMode/ProtoLevel2GameMode.h"
#include "Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"

// Sets default values
ACrack::ACrack()
	: bIsActivate(true)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereCollisionComp->SetupAttachment(SceneComp);
    SphereCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollisionComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
    
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneComp);
    StaticMeshComp->SetCollisionProfileName(TEXT("BlockAll"));
}

void ACrack::BeginPlay()
{
    Super::BeginPlay();

    SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACrack::OnOverlapBegin);
    SphereCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ACrack::OnOverlapEnd);
}

void ACrack::MoveToLevel()
{

}

void ACrack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
    {
        return;
    }

    ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
    ensure(PlayerCharacter);
    CachedCharacter = PlayerCharacter;
    
    PlayerCharacter->SetCurrentInteractableObject(this);
}

void ACrack::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (IsValid(OtherActor) && OtherActor == CachedCharacter)
    {
        ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
        ensure(PlayerCharacter);

        CachedCharacter = nullptr;
        PlayerCharacter->SetCurrentInteractableObject(nullptr);
    }
    else
    {
        Debug::Print(TEXT("ACrack::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
    }
}

void ACrack::Interact_Implementation(AActor* Interactor)
{
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

FText ACrack::GetInteractMessage_Implementation()
{
    return FText::FromString(TEXT("F 키를 눌러 균열과 상호작용"));
}
