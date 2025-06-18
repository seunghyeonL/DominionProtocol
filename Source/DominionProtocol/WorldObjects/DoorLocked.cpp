#include "WorldObjects/DoorLocked.h"
#include "Util/DebugHelper.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/ItemComponent/ItemComponent.h"

ADoorLocked::ADoorLocked()
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

void ADoorLocked::BeginPlay()
{
	Super::BeginPlay();
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoorLocked::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindDynamic(this, &ADoorLocked::OnTimelineFinished);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADoorLocked::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ADoorLocked::OnOverlapEnd);
	
}

void ADoorLocked::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ADoorLocked::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("DoorLocked: Overlap"));

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

void ADoorLocked::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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
		Debug::Print(TEXT("ADoorLocked::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}

void ADoorLocked::OpenDoor(float Value)
{
	FRotator Rot = FRotator(0.f, DoorRotateAngle * Value, 0.f);

	DoorMesh->SetRelativeRotation(Rot);
}

void ADoorLocked::OnTimelineFinished()
{
	DoorMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void ADoorLocked::Interact_Implementation(AActor* Interactor)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
	if (!IsValid(PlayerCharacter))
	{
		Debug::Print(TEXT("ADoorLocked::Interact_Implementation: Interactor is not a valid PlayerCharacter."));
		return;
	}

	UItemComponent* PlayerItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
	if (!IsValid(PlayerItemComponent))
	{
		Debug::Print(TEXT("ADoorLocked::Interact_Implementation: Player's ItemComponent not found!"));
		return;
	}

	DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));
	if (bIsDoorClosed)
	{
		if (PlayerItemComponent->HasItem(RequiredKey, 1)) // 1개 이상 있는지 확인
		{
			// 열쇠가 있다면 문 열기
			Timeline.Play();
			Debug::Print(FString::Printf(TEXT("Door Opened (Key '%s' Found)"), *RequiredKey.ToString()));
			bIsDoorClosed = false;
			//열쇠 소모 방식?
			// PlayerItemComponent->RemoveItem(RequiredKeyTag, 1); 
		}
		else
		{
			// 열쇠가 없다면 문 열기 실패
			Debug::Print(FString::Printf(TEXT("Door Locked (No Key '%s' Found)"), *RequiredKey.ToString()));
		}
	}
	else
	{
		Timeline.Reverse();
		Debug::Print(TEXT("Door Closed"));
		bIsDoorClosed = true;
	}
	
}

FText ADoorLocked::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("문 움직이기"));
}