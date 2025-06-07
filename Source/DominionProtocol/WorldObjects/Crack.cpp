// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Engine/TargetPoint.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "DomiFramework/GameMode/ProtoLevel2GameMode.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "EnumAndStruct/FCrackData.h"
#include "Player/InGameController.h"
#include "Player/Characters/DomiCharacter.h"
#include "UI/UIInGame/DomiInGameHUDWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

#include "Util/DebugHelper.h"

DEFINE_LOG_CATEGORY(LogCrackSystem);

// Sets default values
ACrack::ACrack()
	: CrackName(FText::GetEmpty()),
	  CrackIndex(0),
	  InteractableRadius(150.f),
	  ActivationDistanceCalculateRadius(1500.f),
	  Distance(1600.f),
	  bIsActivate(false),
	  bBlockInteract(false)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BlockSphereComponent"));
	CapsuleComp->SetupAttachment(SceneComp);
	CapsuleComp->SetCollisionProfileName(TEXT("BlockAll"));

	RespawnTargetPointComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("RespawnTargetPoint"));
	RespawnTargetPointComp->SetupAttachment(SceneComp);
	RespawnTargetPointComp->SetChildActorClass(ATargetPoint::StaticClass());
	RespawnTargetPointComp->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

	BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	BGMAudioComponent->SetupAttachment(SceneComp);
}

void ACrack::BeginPlay()
{
	Super::BeginPlay();

	BGMAudioComponent->SetSound(BGMSound);
	BGMAudioComponent->AttenuationSettings = SoundAttenuation;
	BGMAudioComponent->bAutoActivate = true;
	BGMAudioComponent->Play();
	
	DistanceCalculateRadiusSquared = ActivationDistanceCalculateRadius * ActivationDistanceCalculateRadius;
	InteractableRadiusSquared = InteractableRadius * InteractableRadius;

	RespawnTargetPoint = Cast<ATargetPoint>(RespawnTargetPointComp->GetChildActor());

	WorldInstanceSubsystem = GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	check(WorldInstanceSubsystem);

	BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));

	GetWorldTimerManager().SetTimer(
		DetectionTimerHandle,
		this,
		&ACrack::CheckPlayerInActivationRange,
		1.f,
		true);
}

void ACrack::Interact_Implementation(AActor* Interactor)
{
	if (bBlockInteract) return;
	
	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	//최근 균열 업데이트
	BaseGameMode->SetRecentCrackCache(this);

	WorldInstanceSubsystem->SetRecentCrackName(CrackName);
	WorldInstanceSubsystem->SetRecentCrackIndex(CrackIndex);

	// 흐름
	// 1. 비활성화시 활성화
	// 2. 조력자 대화 이벤트(가능한 경우)
	// 2. 기능

	// 1. 해당 균열 활성화
	if (!bIsActivate)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivateSound, GetActorLocation());
		bIsActivate = true;
		WorldInstanceSubsystem->SetIsActivateCrackIndex(WorldInstanceSubsystem->GetCurrentLevelName(), CrackIndex);
		Debug::Print(CrackName.ToString() + TEXT(" 활성화"));
		return;
	}

	// 2. 조력자 대화 이벤트(가능한 경우)

	
	// 3. 기능
	
	// 적 몬스터 All Destroy
	BaseGameMode->DestroyAllNormalEnemy();
	
	auto* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
	if (PlayerCharacter)
	{
		//체력회복
		UStatusComponent* StatusComponent = PlayerCharacter->GetStatusComponent();
		if (IsValid(StatusComponent))
		{
			StatusComponent->SetHealth(BIG_NUMBER);
		}
		
		//회복 포션 개수 -> Max 회복
		UItemComponent* ItemComponent = PlayerCharacter->GetItemComponent();
		if (IsValid(ItemComponent))
		{
			ItemComponent->RestorePotion();
		}

		// 적 몬스터 초기화
		BaseGameMode->RespawnEnemies();
		
		auto* PlayerController = Cast<AInGameController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			UDomiInGameHUDWidget* InGameHUDWidget = PlayerController->GetInGameHUDWidget();

			// 균열 이동
			if (InGameHUDWidget)
			{
				InGameHUDWidget->OnSwitchShowAndHideCrackWarpWidget();
			}

			// 레벨업

			// 회복포션 업그레이트
		}
	}
}

FText ACrack::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("균열과 상호작용"));
}

void ACrack::CheckPlayerInActivationRange()
{
	if (!IsValid(CachedCharacter))
	{
		CachedCharacter = Cast<ADomiCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	Distance = FVector::DistSquared(GetActorLocation(), CachedCharacter->GetActorLocation());

	if (Distance <= DistanceCalculateRadiusSquared)
	{
		SetDetailDetection(true);
	}
	else if (Distance > DistanceCalculateRadiusSquared)
	{
		SetDetailDetection(false);
	}
}

void ACrack::CheckPlayerInInteractionRange()
{
	if (IsValid(CachedCharacter) && !bBlockInteract)
	{
		Distance = FVector::DistSquared(GetActorLocation(), CachedCharacter->GetActorLocation());

		if (Distance <= InteractableRadiusSquared)
		{
			CachedCharacter->AddInteractableActor(this);
		}
		else if (Distance > InteractableRadiusSquared)
		{
			CachedCharacter->RemoveInteractableActor(this);
		}
	}
}

void ACrack::SetDetailDetection(bool bEnable)
{
	if (bEnable)
	{
		GetWorldTimerManager().SetTimer(
			DetailDetectionTimerHandle,
			this,
			&ACrack::CheckPlayerInInteractionRange,
			0.1f,
			true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DetailDetectionTimerHandle);
	}
}

FVector ACrack::GetRespawnTargetPointLocation() const
{
	if (IsValid(RespawnTargetPoint))
	{
		return RespawnTargetPoint->GetActorLocation();
	}
	else
	{
		Debug::PrintError(TEXT("Invalid RespawnTargetPoint"));
		return FVector::ZeroVector;
	}
}

FRotator ACrack::GetRespawnTargetPointRotation() const
{
	if (IsValid(RespawnTargetPoint))
	{
		return RespawnTargetPoint->GetActorRotation();
	}
	else
	{
		Debug::PrintError(TEXT("Invalid RespawnTargetPoint"));
		return FRotator::ZeroRotator;
	}
}
