#include "WorldObjects/BlockedPath.h"
#include "Components/BoxComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Kismet/GameplayStatics.h"

ABlockedPath::ABlockedPath()
	:bIsBlocking(true)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);

	PathEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PathEffect"));
	PathEffect->SetupAttachment(RootComponent);
	PathEffect->SetAutoActivate(false); 

	PathEffect->SetRelativeLocation(FVector::ZeroVector);
	PathEffect->SetWorldLocation(GetActorLocation() + FVector(0, 0, 100.f));
	PathEffect->SetWorldScale3D(FVector(1.f));
}

void ABlockedPath::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (NiagaraTemplate && PathEffect)
	{
		PathEffect->SetAsset(NiagaraTemplate);
		PathEffect->ReinitializeSystem();
		PathEffect->SetVisibility(true, true);
		PathEffect->Activate();
	}
}

void ABlockedPath::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (PathEffect && NiagaraTemplate)
			{
				PathEffect->SetAsset(NiagaraTemplate);
				PathEffect->ReinitializeSystem();   // 다시 초기화
				PathEffect->SetVisibility(true, true);
				PathEffect->Activate();             // 늦게 실행
			}
		}, 0.1f, false);

	//TryOpen();
}

void ABlockedPath::TryOpen()
{
	auto GameInstance = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance && GameInstance->GetCurrentGameStoryState() >= RequiredStoryState)
	{
		bIsBlocking = false;
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PathEffect->Deactivate();

		OnOpened();
	}
}
