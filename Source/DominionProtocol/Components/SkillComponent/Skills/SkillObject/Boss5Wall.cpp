#include "Components/SkillComponent/Skills/SkillObject/Boss5Wall.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Characters/DomiCharacter.h"
#include "Boss5Wall.h"

ABoss5Wall::ABoss5Wall()
{
    PrimaryActorTick.bCanEverTick = true;

    // 벽 메쉬
    WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
    WallMesh->SetupAttachment(RootComponent);
    WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    WallMesh->SetCollisionResponseToAllChannels(ECR_Block);
    WallMesh->SetCollisionObjectType(ECC_WorldStatic);
    WallMesh->SetGenerateOverlapEvents(false); // 벽 자체는 오버랩 없음

    // 푸시 트리거
    PushTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PushTrigger"));
    PushTrigger->SetupAttachment(WallMesh);
    PushTrigger->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    PushTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PushTrigger->SetCollisionObjectType(ECC_WorldDynamic);
    PushTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    PushTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PushTrigger->SetGenerateOverlapEvents(true);

    PushTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABoss5Wall::OnPushTriggerBeginOverlap);
    PushTrigger->OnComponentEndOverlap.AddDynamic(this, &ABoss5Wall::OnPushTriggerEndOverlap);

    // 초기 상태 설정
    bShouldPush = false;
    PushSpeed = 1500.f;
    PushProgress = 0.f;
    LastPushTime = -FLT_MAX;
    PushCooldown = 8.f;
}

void ABoss5Wall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bShouldPush && !bShouldReturn)
    {
        Push();
        return;
    }

    FVector From = bShouldPush ? StartLocation : EndLocation;
    FVector To = bShouldPush ? EndLocation : StartLocation;

    float MovementAlpha = (PushSpeed * DeltaTime) / (To - From).Size();
    PushProgress = FMath::Clamp(PushProgress + MovementAlpha, 0.f, 1.f);

    FVector NewLocation = FMath::Lerp(From, To, PushProgress);
    FVector MoveDelta = NewLocation - GetActorLocation();

    SetActorLocation(NewLocation);

    if (bShouldPush && OverlappingCharacter.IsValid())
    {
        OverlappingCharacter->AddActorWorldOffset(MoveDelta, true);
    }

    if (PushProgress >= 1.f)
    {
        if (bShouldPush)
        {
            bShouldPush = false;
            bShouldReturn = true;
            PushProgress = 0.f;
        }
        else if (bShouldReturn)
        {
            bShouldReturn = false;
            OverlappingCharacter = nullptr;
        }
    }
}

void ABoss5Wall::Push()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastPushTime < PushCooldown)
    {
        return;
    }

    float TraceLength = 1000.f;

    FVector Forward = GetActorRightVector();
    FVector TraceStart = WallMesh->Bounds.Origin;
    FVector TraceEnd = TraceStart + Forward * TraceLength;

    FVector BoxSize = PushTrigger->GetScaledBoxExtent();

    FHitResult Hit;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(this);

    UKismetSystemLibrary::BoxTraceSingleForObjects(
        this,
        TraceStart,
        TraceEnd,
        BoxSize,
        WallMesh->GetComponentRotation(),
        ObjectTypes,
        false,
        IgnoreActors,
        EDrawDebugTrace::None,
        Hit,
        true
    );


    if (!Hit.bBlockingHit) return;

    ADomiCharacter* TargetActor = Cast<ADomiCharacter>(Hit.GetActor());
    if (!TargetActor) return;

    bShouldPush = true;
    bShouldReturn = false;
    StartLocation = GetActorLocation();
    EndLocation = StartLocation + Forward * TraceLength;
    PushProgress = 0.f;

    LastPushTime = CurrentTime;
}

void ABoss5Wall::OnPushTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OtherActor))
    {
        OverlappingCharacter = DomiChar;
    }
}

void ABoss5Wall::OnPushTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OtherActor))
    {
        if (OverlappingCharacter == DomiChar)
        {
            OverlappingCharacter = nullptr;
        }
    }
}