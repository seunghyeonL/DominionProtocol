#include "WorldObjects/BlockedPath.h"
#include "Components/BoxComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Kismet/GameplayStatics.h"

ABlockedPath::ABlockedPath()
	:bIsBlocking(true)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);

	PathEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PathEffect"));
	PathEffect->SetupAttachment(RootComponent);
	PathEffect->SetAutoActivate(false); 

	PathEffect->SetRelativeLocation(FVector::ZeroVector);
	PathEffect->SetWorldLocation(GetActorLocation() + FVector(0, 0, 100.f));
	PathEffect->SetWorldScale3D(FVector(1.f));
}

void ABlockedPath::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	UE_LOG(LogTemp, Error, TEXT("BlockedPath::OnStoryStateUpdated_Implementation"));
	if (NewState >= RequiredStoryState)
	{
		TryOpen();
	}
}

void ABlockedPath::BeginPlay()
{
	Super::BeginPlay();

	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->OnStoryStateChanged.AddDynamic(this, &ABlockedPath::OnStoryStateUpdated);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (PathEffect && NiagaraTemplate)
			{
				PathEffect->SetAsset(NiagaraTemplate);
				PathEffect->ReinitializeSystem();
				PathEffect->SetVisibility(true, true);
				PathEffect->Activate();
			}
		}, 0.1f, false);

	if (!CollisionBox || !PathEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("BlockedPath::BeginPlay - 구성 요소가 초기화되지 않음!"));
		return;
	}

	TryOpen();
}

void ABlockedPath::TryOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("BlockedPath: TryOpen"));
	if (!IsValid(CollisionBox))
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockedPath: CollisionBox is null"));
		return;
	}

	if (!IsValid(PathEffect))
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockedPath: PathEffect is null"));
		return;
	}
	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance && GameInstance->GetCurrentGameStoryState() >= RequiredStoryState)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockedPath: 조건 충족, 열기"));
		bIsBlocking = false;
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PathEffect->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockedPath: 조건 미충족, 막힘 유지"));
	}
}