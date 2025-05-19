// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/TargetPoint.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "DomiFramework/GameMode/ProtoLevel2GameMode.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "EnumAndStruct/FCrackData.h"
#include "Player/InGameController.h"
#include "Player/Characters/DomiCharacter.h"
#include "UI/UIInGame/DomiInGameHUDWidget.h"

#include "Util/DebugHelper.h"

DEFINE_LOG_CATEGORY(LogCrackSystem);

// Sets default values
ACrack::ACrack()
	:   CrackName(FText::GetEmpty()),
        CrackIndex(0),
        bIsActivate(false)
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
    
    WorldInstanceSubsystem = GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
    check(WorldInstanceSubsystem);
    
    BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
    
    SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACrack::OnOverlapBegin);
    SphereCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ACrack::OnOverlapEnd);
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
    
    PlayerCharacter->AddInteractableActor(this);
}

void ACrack::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (IsValid(OtherActor) && OtherActor == CachedCharacter)
    {
        ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
        ensure(PlayerCharacter);

        CachedCharacter = nullptr;
        PlayerCharacter->RemoveInteractableActor(this);
    }
    else
    {
        Debug::Print(TEXT("ACrack::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
    }
}

void ACrack::Interact_Implementation(AActor* Interactor)
{
    UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
    if (!GameInstance) return;

    //최근 균열 업데이트
    BaseGameMode->SetRecentCrackCache(this);
    
    WorldInstanceSubsystem->SetRecentCrackName(CrackName);
    WorldInstanceSubsystem->SetRecentCrackIndex(CrackIndex);
    
    // 흐름
    // 1. 비활성화시 활성화
    // 2. 기능
    
    // 1. 해당 균열 활성화
    if (!bIsActivate)
    {
        bIsActivate = true;
        WorldInstanceSubsystem->SetIsActivateCrackIndex(WorldInstanceSubsystem->GetCurrentLevelName(), CrackIndex);
        UE_LOG(LogCrackSystem, Warning, TEXT("%s 활성화"), *CrackName.ToString());
        Debug::Print(CrackName.ToString()+TEXT(" 활성화"));
        return;
    }
    
    // 2. 기능
    auto* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
    if (PlayerCharacter)
    {
        auto* PlayerController = Cast<AInGameController>(PlayerCharacter->GetController());
        if (PlayerController)
        {
            UDomiInGameHUDWidget* InGameHUDWidget = PlayerController->GetInGameHUDWidget();
            if (InGameHUDWidget)
            {
                InGameHUDWidget->OnSwitchShowAndHideCrackWarpWidget();
            }
        }
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

FRotator ACrack::GetRespawnTargetPointRotation() const
{
    if (IsValid(RespawnTargetPoint))
    {
        return RespawnTargetPoint->GetActorRotation();
    }
    else
    {
        Debug::PrintError(TEXT("Invalid RespawnTargetPoint"));
        return FRotator::ZeroRotator;
    }
}
