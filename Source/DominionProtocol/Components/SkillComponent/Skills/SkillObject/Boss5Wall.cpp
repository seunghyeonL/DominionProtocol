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
    SetRootComponent(WallMesh);

    // 푸시 트리거
    PushTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PushTrigger"));
    PushTrigger->SetupAttachment(WallMesh);
    PushTrigger->SetRelativeLocation(FVector(200.f, 40.f, 150.f));
    PushTrigger->SetGenerateOverlapEvents(true);

    PushTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABoss5Wall::OnPushTriggerBeginOverlap);
    PushTrigger->OnComponentEndOverlap.AddDynamic(this, &ABoss5Wall::OnPushTriggerEndOverlap);

    CheckTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckTrigger"));
    CheckTrigger->SetupAttachment(WallMesh);
    CheckTrigger->SetRelativeLocation(FVector(200.f, 40.f, 150.f));
    CheckTrigger->SetGenerateOverlapEvents(true);

    CheckTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABoss5Wall::OnCheckTriggerBeginOverlap);
    CheckTrigger->OnComponentEndOverlap.AddDynamic(this, &ABoss5Wall::OnCheckTriggerEndOverlap);

    // 초기 상태 설정
    bShouldPush = false;
    bShouldReturn = false;
    PushSpeed = 100.f;
    PushProgress = 0.f;
    LastPushTime = -FLT_MAX;
    PushCooldown = 5.f;
}

void ABoss5Wall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bShouldPush && !bShouldReturn)
    {    
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

<<<<<<< HEAD
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

=======
>>>>>>> 2d835bb6 ([update] 보스5 1층 외곽벽 tick->오버랩으로 변경 및 보스5 테스트맵 업데이트)
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

void ABoss5Wall::OnCheckTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OtherActor))
    {
        CurrentTime = GetWorld()->GetTimeSeconds();
        if (CurrentTime - LastPushTime < PushCooldown)
        {
            return;
        }

        FVector Forward = GetActorRightVector();
        float Length = 1000.f;

        bShouldPush = true;
        bShouldReturn = false;
        StartLocation = GetActorLocation();
        EndLocation = StartLocation + Forward * Length;
        PushProgress = 0.f;
        LastPushTime = CurrentTime;
    }
}

void ABoss5Wall::OnCheckTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}