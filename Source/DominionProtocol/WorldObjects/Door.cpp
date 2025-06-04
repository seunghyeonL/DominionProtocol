#include "WorldObjects/Door.h"
#include "Util/DebugHelper.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"

ADoor::ADoor()
	: bIsDoorClosed(true),
	DoorRotateAngle(90.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(SceneComp);
	DoorMesh->SetCollisionProfileName(TEXT("BlockAll"));

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollisionComp->SetupAttachment(SceneComp);
	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor::MoveDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindDynamic(this, &ADoor::OnTimelineFinished);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("Door: Overlap"));

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

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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
		Debug::Print(TEXT("ADoor::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}

void ADoor::MoveDoor(float Value)
{
	FRotator Rot = FRotator(0.f, DoorRotateAngle * Value, 0.f);

	DoorMesh->SetRelativeRotation(Rot);
}

void ADoor::OpenDoor()
{
	if (!bIsDoorClosed || Timeline.IsPlaying()) return;

	DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Timeline.Play();
	bIsDoorClosed = false;
}

void ADoor::CloseDoor()
{
	if (bIsDoorClosed || Timeline.IsPlaying()) return;

	DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Timeline.Reverse();
	bIsDoorClosed = true;
}

void ADoor::OnTimelineFinished()
{
	DoorMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void ADoor::Interact_Implementation(AActor* Interactor)
{
	DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));
	if (bIsDoorClosed)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

FText ADoor::GetInteractMessage_Implementation() const
{
	return bIsDoorClosed
		? FText::FromString(TEXT("문 열기"))
		: FText::FromString(TEXT("문 닫기"));
}