// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TargetPoint.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Player/InGameController.h"
#include "Player/Characters/DomiCharacter.h"
#include "UI/UIInGame/DomiInGameHUDWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "WorldObjects/DialogueManager.h"

#include "Util/DebugHelper.h"

DEFINE_LOG_CATEGORY(LogCrackSystem);

// Sets default values
ACrack::ACrack()
	: CrackName(FText::GetEmpty()),
	  CrackIndex(0),
	  RequiredRevealStoryState(EGameStoryState::Tutorial),
	  bIsBossRoomCrack(false),
	  bIsInFogCrack(false),
	  bIsBoss3Crack(false),
	  bShouldOffSkyAtmosphere(false),
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

	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	check(IsValid(GameInstance));

	WorldInstanceSubsystem = GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	check(IsValid(WorldInstanceSubsystem));

	BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
	check(IsValid(BaseGameMode));

	if (bIsBossRoomCrack)
	{
		GameInstance->OnStoryStateChanged.AddDynamic(this, &ACrack::OnStoryStateChanged);
	}

	GetWorldTimerManager().SetTimer(
		DetectionTimerHandle,
		this,
		&ACrack::CheckPlayerInActivationRange,
		1.f,
		true);
}

void ACrack::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (IsValid(GameInstance))
	{
		GameInstance->OnStoryStateChanged.RemoveDynamic(this, &ACrack::OnStoryStateChanged);
	}
}

void ACrack::Interact_Implementation(AActor* Interactor)
{
	if (bBlockInteract)
	{
		return;
	}

	//최근 균열 업데이트
	BaseGameMode->SetRecentCrackCache(this);

	WorldInstanceSubsystem->SetRecentCrackName(CrackName);
	WorldInstanceSubsystem->SetRecentCrackIndex(CrackIndex);

	if (!IsValid(CachedCharacter))
	{
		CachedCharacter = Cast<ADomiCharacter>(Interactor);
	}
	check(IsValid(CachedCharacter));

	AInGameController* PlayerController = Cast<AInGameController>(CachedCharacter->GetController());
	check(IsValid(PlayerController));

	// 흐름
	// 1. 비활성화시 활성화
	// 2. 조력자 대화 이벤트
	// 2. 기능

	// 1. 해당 균열 활성화
	if (!bIsActivate)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivateSound, GetActorLocation());
		bIsActivate = true;
		WorldInstanceSubsystem->SetIsActivateCrackIndex(WorldInstanceSubsystem->GetCurrentLevelName(), CrackIndex);
		// 저장
		BaseGameMode->Save();

		OnActiveCrack.Broadcast(CrackName);
		Debug::Print(CrackName.ToString() + TEXT(" 활성화"));
		return;
	}

	// 2. 조력자 대화 이벤트
	DialogueManager = NewObject<UDialogueManager>(this);
	OnCreateDialogueManager.Broadcast(DialogueManager);
	DialogueManager->HelperClass = HelperClass;
	FVector CrackLocation = GetActorLocation();
	FRotator CrackRotation = GetActorRotation();
	if (DialogueManager->TryStartDialogueIfExists(GameInstance->GetCurrentGameStoryState(), CrackLocation, CrackRotation))
	{
		if (CachedCharacter)
		{
			APlayerController* PC = Cast<APlayerController>(CachedCharacter->GetController());
			if (PC)
			{
				if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					GM->SetPlayerInputEnable(true);
					Debug::Print(TEXT("ACrack: SetPlayerInputEnable true"));
				}
				else
				{
					Debug::Print(TEXT("ACrack: ABaseGameMode NotValid"));
				}
			}
			else
			{
				Debug::Print(TEXT("ACrack: APlayerController NotValid"));
			}
		}
		// AlignPlayerForDialogue(Cast<ADomiCharacter>(Interactor));
		Debug::Print(TEXT("Crack: 조력자 이벤트 종료"));
		GameInstance->AdvanceStoryState();

		return;
	}
	else
	{
		Debug::Print(TEXT("Crack: 조력자 이벤트 없음"));
	}

	// 3. 기능

	// 적 몬스터 All Destroy
	BaseGameMode->DestroyAllNormalEnemy();
	// 플레이어 체력 및 포션 개수 회복
	BaseGameMode->RestorePlayer();
	// 적 몬스터 초기화
	BaseGameMode->RespawnEnemies();

	BaseGameMode->Save();
	
	UDomiInGameHUDWidget* InGameHUDWidget = PlayerController->GetInGameHUDWidget();

	// 균열 이동
	if (InGameHUDWidget)
	{
		InGameHUDWidget->OnShowCrackMenuWidget();
	}

	// 레벨업

	// 회복포션 업그레이트
}

FText ACrack::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("균열과 상호작용"));
}

void ACrack::OnStoryStateChanged(EGameStoryState NewState)
{
	bool bIsBossSameLinkedBossClass = false;

	if (static_cast<int32>(NewState) < static_cast<int32>(RequiredRevealStoryState))
	{
		SetActorHiddenInGame(true);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BGMAudioComponent->Stop();
		bBlockInteract = true;
	}
	else
	{
		SetActorHiddenInGame(false);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BGMAudioComponent->Play();
		bBlockInteract = false;
	}
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
	if (IsValid(RespawnTargetPointComp->GetChildActor()))
	{
		return RespawnTargetPointComp->GetChildActor()->GetActorLocation();
	}
	else
	{
		Debug::PrintError(TEXT("Invalid RespawnTargetPoint"));
		return FVector::ZeroVector;
	}
}

FRotator ACrack::GetRespawnTargetPointRotation() const
{
	if (IsValid(RespawnTargetPointComp->GetChildActor()))
	{
		return RespawnTargetPointComp->GetChildActor()->GetActorRotation();
	}
	else
	{
		Debug::PrintError(TEXT("Invalid RespawnTargetPoint"));
		return FRotator::ZeroRotator;
	}
}

void ACrack::AlignPlayerForDialogue(ADomiCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	const FVector CrackForward = GetActorForwardVector();
	const FVector Offset = FVector(100.f, 300.f, -50.f);
	const FVector TargetLocation = GetActorLocation() + Offset;

	const FRotator TargetRotation = (GetActorLocation() - TargetLocation).Rotation();

	PlayerCharacter->SetActorLocation(TargetLocation);
	PlayerCharacter->SetActorRotation(TargetRotation);

	if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		PC->SetControlRotation(TargetRotation);
	}

	//////////////////
	/*
	if (!PlayerCharacter) return;

	const FVector CrackForward = GetActorForwardVector();
	const FVector DesiredLocation = GetActorLocation() + CrackForward * 200.f;

	FNavLocation SafeLocation;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSys && NavSys->ProjectPointToNavigation(
		DesiredLocation,
		SafeLocation,
		FVector(100.f, 100.f, 200.f))) // 탐색 반경
	{
		PlayerCharacter->SetActorLocation(SafeLocation.Location);

		const FRotator TargetRotation = (GetActorLocation() - SafeLocation.Location).Rotation();
		PlayerCharacter->SetActorRotation(TargetRotation);

		if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			PC->SetControlRotation(TargetRotation);
		}
		Debug::Print(TEXT("플레이어를 NavMesh 안전 위치로 이동시켰습니다"));
	}
	else
	{
		Debug::Print(TEXT("NavMesh 기반 안전 위치 탐색 실패: 위치 이동 안 함"));
	}
	*/
}