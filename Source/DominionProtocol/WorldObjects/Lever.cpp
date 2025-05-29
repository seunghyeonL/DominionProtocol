#include "WorldObjects/Lever.h"
#include "Util/DebugHelper.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"

ALever::ALever()
	:bIsOn(false),
	LeverRotateAngle(60)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	LeverMesh->SetupAttachment(SceneComp);
	LeverMesh->SetCollisionProfileName(TEXT("BlockAll"));

	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
	HandleMesh->SetupAttachment(SceneComp);
	HandleMesh->SetCollisionProfileName(TEXT("BlockAll"));

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollisionComp->SetupAttachment(SceneComp);
	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InitialRotation = HandleMesh->GetRelativeRotation();
}

void ALever::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ALever::HandleLever);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindDynamic(this, &ALever::OnTimelineFinished);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnOverlapBegin);
	BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ALever::OnOverlapEnd);
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ALever::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("Lever: Overlap"));

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

void ALever::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
		Debug::Print(TEXT("Lever::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}

void ALever::HandleLever(float Value)
{
	//FRotator Rot = FRotator(0.f, LeverRotateAngle * Value, 0.f);

	//HandleMesh->SetRelativeRotation(Rot);

	FRotator DeltaRotation = FRotator(LeverRotateAngle * Value, 0.f, 0.f);
	FRotator NewRotation = InitialRotation + DeltaRotation;
	HandleMesh->SetRelativeRotation(NewRotation);
}
void ALever::OnTimelineFinished()
{
}

void ALever::Interact_Implementation(AActor* Interactor)
{
	if (bIsOn)
	{
		Timeline.Reverse();
		Debug::Print(TEXT("Lever Off"));
	}
	else
	{
		Timeline.Play();
		Debug::Print(TEXT("Lever On"));
		if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(GetGameInstance()))
		{
			GI->SetCurrentGameStoryState(EGameStoryState::OperateLever); 
			Debug::Print(TEXT("스토리 상태 → OperateLever 으로 변경"));
		}
	}
	bIsOn = !bIsOn;
}

FText ALever::GetInteractMessage_Implementation() const
{
	return bIsOn
		? FText::FromString(TEXT("F 키를 눌러 레버 활성화"))
		: FText::FromString(TEXT("F 키를 눌러 레버 비활성화"));
}