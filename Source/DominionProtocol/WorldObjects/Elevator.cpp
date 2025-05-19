#include "WorldObjects/Elevator.h"
#include "Util/DebugHelper.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"

AElevator::AElevator()
	: bIsMoving(false),
	bIsAtTop(false),
	MoveDistance(100.f)
{
	PrimaryActorTick.bCanEverTick = true;

	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + FVector(0.f, 0.f, MoveDistance);

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComp);
	FloorMesh->SetCollisionProfileName(TEXT("BlockAll"));

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollisionComp->SetupAttachment(SceneComp);
	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Wall = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall"));
	Wall->SetupAttachment(RootComponent);

	Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	Debug::Print(TEXT("AElevator: Overlap"));

	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("Not Player"));
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
	else
	{
		Debug::Print(TEXT("AElevator::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}


void AElevator::ActivateElevator(float Value)
{
	FVector NewLoc = FMath::Lerp(StartLocation, TargetLocation, Value);
	SetActorLocation(NewLoc);
}

void AElevator::OnTimelineFinished()
{
	Wall->SetCollisionProfileName(TEXT("NoCollision")); 
	bIsAtTop = !bIsAtTop;
	bIsMoving = false;
}


void AElevator::Interact_Implementation(AActor* Interactor)
{
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	if (bIsMoving)
	{
		Debug::Print(TEXT("Elevator is moving – interaction blocked"));
		return;
	}

	FVector CurrentLocation = GetActorLocation();

	if (bIsAtTop)
	{
		StartLocation = CurrentLocation;
		TargetLocation = CurrentLocation - FVector(0.f, 0.f, MoveDistance);
		Timeline.PlayFromStart();
		Debug::Print(TEXT("Elevator Moving Down"));
	}
	else
	{
		StartLocation = CurrentLocation;
		TargetLocation = CurrentLocation + FVector(0.f, 0.f, MoveDistance);
		Timeline.PlayFromStart();
		Debug::Print(TEXT("Elevator Moving Up"));
	}

	bIsMoving = true;
}

FText AElevator::GetInteractMessage_Implementation() const
{
	if (bIsMoving)
	{
		return FText::GetEmpty();
	}

	return bIsAtTop
		? FText::FromString(TEXT("F 키를 눌러 내려가기"))
		: FText::FromString(TEXT("F 키를 눌러 올라가기"));
}
