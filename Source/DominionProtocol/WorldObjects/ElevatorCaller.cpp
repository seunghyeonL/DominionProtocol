#include "WorldObjects/ElevatorCaller.h"
#include "Util/DebugHelper.h"
#include "WorldObjects/Elevator.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"

AElevatorCaller::AElevatorCaller()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	TriggerMesh->SetupAttachment(SceneComp);

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollisionComp->SetupAttachment(SceneComp);
	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AElevatorCaller::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AElevatorCaller::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AElevatorCaller::OnOverlapEnd);
}

void AElevatorCaller::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(Elevator))
	{
		Debug::Print(TEXT("Elevator not set"));
		return;
	}
	if (Elevator->IsMoving())
	{
		Debug::Print(TEXT("Elevator is Moving"));
		return;
	}
	Elevator->MoveElevatorTo(GetActorLocation().Z);
}

FText AElevatorCaller::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("엘레베이터 부르기"));
}

void AElevatorCaller::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("AElevatorCaller: Overlap"));

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

void AElevatorCaller::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
		Debug::Print(TEXT("AElevatorCaller::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}