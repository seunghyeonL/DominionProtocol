// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/TargetPoint.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "DomiFramework/GameMode/ProtoLevel2GameMode.h"
#include "Player/Characters/DomiCharacter.h"

#include "Util/DebugHelper.h"

// Sets default values
ACrack::ACrack()
	:   CrackName(FText::GetEmpty()),
        CrackIndex(0),
        bIsActivate(true)
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

    RespawnTargetPointComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("RespawnTargetPoint"));
    RespawnTargetPointComp->SetupAttachment(SceneComp);
    RespawnTargetPointComp->SetChildActorClass(ATargetPoint::StaticClass());
    RespawnTargetPointComp->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
}

void ACrack::BeginPlay()
{
    Super::BeginPlay();

    RespawnTargetPoint = Cast<ATargetPoint>(RespawnTargetPointComp->GetChildActor());
    
    //***프로토타입용 코드*** 본 개발시 변경해야함
    UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
    check(GameInstance);

    GameInstance->SetRecentCrackName(CrackName);
    GameInstance->SetRecentCrackIndex(CrackIndex);
    //***================================***
    
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

FText ACrack::GetInteractMessage_Implementation() const
{
    return FText::FromString(TEXT("F 키를 눌러 균열과 상호작용"));
}

FVector ACrack::GetRespawnTargetPointLocation() const
{
    if (IsValid(RespawnTargetPoint))
    {
        return RespawnTargetPoint->GetActorLocation();
    }
    else
    {
        Debug::PrintError(TEXT("Invalid RespawnTargetPoint"));
        return FVector::ZeroVector;
    }
}