// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss3Skull.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Player/Characters/DomiCharacter.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"

#include "Util/DebugHelper.h"

ABoss3Skull::ABoss3Skull()
	: ShakeTimeDuration(0.02f),
	  MaxShakeTime(2.f),
	  StoryStateCache(EGameStoryState::Tutorial),
	  bIsInBattleRoom(false),
	  TimeCount(0.f)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneRootComponent);

	SkullMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkullStaticMesh"));
	SkullMeshComponent->SetupAttachment(SceneRootComponent);
	SkullMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	AltarMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AltarStaticMesh"));
	AltarMeshComponent->SetupAttachment(SceneRootComponent);
	AltarMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	InteractableCollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractableCollisionSphere"));
	InteractableCollisionSphereComponent->SetupAttachment(SceneRootComponent);
	InteractableCollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractableCollisionSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractableCollisionSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABoss3Skull::SetIsInteractable(bool bNewIsInteractable)
{
	if (bNewIsInteractable)
	{
		InteractableCollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractableCollisionSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractableCollisionSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		InteractableCollisionSphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABoss3Skull::OnOverlapBegin);
		InteractableCollisionSphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ABoss3Skull::OnOverlapEnd);
	}
	else
	{
		InteractableCollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractableCollisionSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		InteractableCollisionSphereComponent->OnComponentBeginOverlap.Clear();
		InteractableCollisionSphereComponent->OnComponentEndOverlap.Clear();
	}
}

void ABoss3Skull::BeginPlay()
{
	Super::BeginPlay();

	InteractableCollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABoss3Skull::OnOverlapBegin);
	InteractableCollisionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABoss3Skull::OnOverlapEnd);

	BaseRotation = GetActorRotation();
}

void ABoss3Skull::Interact_Implementation(AActor* Interactor)
{
	ShakeStartTime = GetWorld()->GetTimeSeconds();

	UDomiGameInstance* GameInstance = GetWorld()->GetGameInstance<UDomiGameInstance>();
	check(IsValid(GameInstance));
	
	if (!IsValid(CachedCharacter))
	{
		CachedCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	check(IsValid(CachedCharacter));

	GameInstance->AdvanceStoryState();

	CachedCharacter->GetController()->SetIgnoreMoveInput(true);
	CachedCharacter->GetController()->SetIgnoreLookInput(true);
	
	GetWorldTimerManager().SetTimer(
		ShakeTimerHandle,
		this,
		&ABoss3Skull::OnShake,
		ShakeTimeDuration,
		true);
}

FText ABoss3Skull::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("건드려 본다"));
}

void ABoss3Skull::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	StoryStateCache = NewState;

	if (!bIsInBattleRoom)
	{
		if (static_cast<int32>(NewState) >= static_cast<int32>(EGameStoryState::Clear_Boss3))
		{
			SkullMeshComponent->SetVisibility(false);
			SkullMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetIsInteractable(false);
		}
		else
		{
			SetIsInteractable(true);
			if (!SkullMeshComponent->IsVisible())
			{
				SkullMeshComponent->SetVisibility(true);
				SkullMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
			}
			if (!AltarMeshComponent->IsVisible())
			{
				AltarMeshComponent->SetVisibility(true);
				AltarMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
			}
		}
	}
}

void ABoss3Skull::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	if (!IsValid(CachedCharacter))
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);
		CachedCharacter = PlayerCharacter;
	}

	CachedCharacter->AddInteractableActor(this);
}

void ABoss3Skull::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		if (!IsValid(CachedCharacter))
		{
			ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
			ensure(PlayerCharacter);
			CachedCharacter = PlayerCharacter;
		}
		
		CachedCharacter->RemoveInteractableActor(this);
	}
}

void ABoss3Skull::OnShake()
{
	float Time = GetWorld()->GetTimeSeconds() - ShakeStartTime;
	
	TimeCount += ShakeTimeDuration;
	if (TimeCount >= MaxShakeTime)
	{
		SpawnBoss3();
		GetWorldTimerManager().ClearTimer(ShakeTimerHandle);
		TimeCount = 0.f;
		return;
	}
	
	FRotator ShakeRotator = FRotator(
	FMath::PerlinNoise1D(Time * 25.f) * 50.f,
	FMath::PerlinNoise1D((Time + 50.f) * 5.f) * 100.f,
		0.f);

	SkullMeshComponent->SetRelativeRotation(BaseRotation + ShakeRotator);
}

void ABoss3Skull::SpawnBoss3()
{
	if (!IsValid(CachedCharacter))
	{
		CachedCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	check(IsValid(CachedCharacter));
	
	UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
	check(IsValid(AnimInstance));
	
	ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
	check(IsValid(BaseGameMode));
	BaseGameMode->ToggleBoss3BattleRoom(true);
	
	//보스와 플레이어 위치 고정위한 저장
	FVector BossLocation = GetActorLocation();
	FRotator BossRotation = GetActorRotation();
	FVector PlayerLocation = CachedCharacter->GetActorLocation();
	FRotator PlayerRotation = CachedCharacter->GetActorRotation();

	// 나이아가라 VFX 생성
	for (UNiagaraSystem* Effect : NiagaraSystems)
	{
		if (Effect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				Effect,
				GetActorLocation(),
				GetActorRotation());
		}
	}
	
	//보스 스폰 start
	if (!BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - !BossClass || !BossTag.IsValid()"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossClass, GetActorLocation(),	GetActorRotation(), SpawnParams);

	if (SpawnedBoss)
	{
		SpawnedBoss->SetActorLocation(BossLocation);
		SpawnedBoss->SetActorRotation(BossRotation);
		BaseGameMode->StartBattle(SpawnedBoss);
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
	}
	else
	{
		Debug::PrintError(TEXT("ABoss3Skull::SpawnBoss3 : Boss3 Spawn failed"));
	}
	//보스 스폰 end

	//플레이어 넉백 및 넉백 애님몽타주 실행 start
	CachedCharacter->SetActorLocation(PlayerLocation);
	CachedCharacter->SetActorRotation(PlayerRotation);
	
	FVector LaunchDirection = (CachedCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator LookAtBossRotation = (-LaunchDirection).Rotation();
	CachedCharacter->SetActorRotation(LookAtBossRotation);

	if (IsValid(KnockbackMontage))
	{
		AnimInstance->Montage_Play(KnockbackMontage);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABoss3Skull::OnKnockbackMontageEnded);
	}
	//플레이어 넉백 및 넉백 애님몽타주 실행 end
}

void ABoss3Skull::OnKnockbackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != KnockbackMontage)
	{
		Debug::PrintError(TEXT("ABoss3Skull::OnKnockbackMontageEnded : Montage is not KnockbackMontage"));
		return;
	}
	
	UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
	check(IsValid(AnimInstance));

	AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABoss3Skull::OnKnockbackMontageEnded);

	CachedCharacter->GetController()->SetIgnoreMoveInput(false);
	CachedCharacter->GetController()->SetIgnoreLookInput(false);
}