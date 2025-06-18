#include "WorldObjects/Elevator.h"
#include "Util/DebugHelper.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "WorldObjects/ElevatorCaller.h"

AElevator::AElevator()
	:
	MoveDistance(100.f),
	bIsMoving(false),
	bIsAtTop(false)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComp);
	FloorMesh->SetCollisionProfileName(TEXT("BlockAll"));

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollisionComp->SetupAttachment(SceneComp);
	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AElevator::ActivateElevator);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindDynamic(this, &AElevator::OnTimelineFinished);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnOverlapEnd);
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void AElevator::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("Not Player"));
		return;
	}

	if (bIsMoving)
	{
		return;
	}

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	ensure(PlayerCharacter);
	CachedCharacter = PlayerCharacter;

	PlayerCharacter->AddInteractableActor(this);
}

void AElevator::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);

		CachedCharacter = nullptr;
		PlayerCharacter->RemoveInteractableActor(this);
	}
}


void AElevator::ActivateElevator(float Value)
{
	FVector NewLoc = FMath::Lerp(StartLocation, TargetLocation, Value);
	SetActorLocation(NewLoc);
}

void AElevator::OnTimelineFinished()
{
	bIsAtTop = !bIsAtTop;
	bIsMoving = false;
}

void AElevator::MoveElevatorTo(float TargetZ)
{
	FVector CurrentLocation = GetActorLocation();
	if (FMath::IsNearlyEqual(CurrentLocation.Z, TargetZ, 1.0f))
	{
		Debug::Print(TEXT("Elevator is already at the target location – ignoring movement"));
		return;
	}
	if (bIsMoving)
	{
		Debug::Print(TEXT("Elevator is moving – movement blocked"));
		return;
	}

	
	StartLocation = CurrentLocation;
	TargetLocation = FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ);

	Timeline.PlayFromStart();
	bIsMoving = true;

	Debug::Print(FString::Printf(TEXT("Elevator Moving To Z: %.2f"), TargetZ));
}


void AElevator::Interact_Implementation(AActor* Interactor)
{
	if (bIsMoving)
	{
		Debug::Print(TEXT("Elevator is moving – cannot interact"));
		return;
	}
	if (bIsAtTop && !IsValid(BottomCaller))
	{
		Debug::Print(TEXT("BottomCaller not assigned!"));
		return;
	}
	else if (!bIsAtTop && !IsValid(TopCaller))
	{
		Debug::Print(TEXT("TopCaller not assigned!"));
		return;
	}

	float TargetZ = bIsAtTop
		? BottomCaller->GetActorLocation().Z
		: TopCaller->GetActorLocation().Z;

	MoveElevatorTo(TargetZ);
}

FText AElevator::GetInteractMessage_Implementation() const
{
	if (bIsMoving)
	{
		return FText::GetEmpty();
	}

	return FText::FromString(TEXT("엘레베이터 작동"));
}