#include "WorldObjects/BossCloudDoor.h"
#include "Components/BoxComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"
#include "WorldObjects/BossSpawner.h"
#include "DomiFramework/GameMode/BaseGameMode.h"

ABossCloudDoor::ABossCloudDoor()
	: bIsMontagePlaying(false)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	BlockingBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blocking"));
	BlockingBox->SetupAttachment(RootComponent);
	BlockingBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BlockingBox->SetCollisionResponseToAllChannels(ECR_Block);

	PathEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PathEffect"));
	PathEffect->SetupAttachment(RootComponent);
	PathEffect->SetAutoActivate(true);

	PathEffect->SetRelativeLocation(FVector::ZeroVector);
	PathEffect->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PathEffect->SetWorldScale3D(FVector(1.f));

	EntryTeleportTarget = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportPoint"));
	EntryTeleportTarget->SetupAttachment(RootComponent);
}

void ABossCloudDoor::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	const bool bTooEarly = (NewState < ActiveFromState);
	const bool bValid = (NewState >= ActiveFromState && NewState <= ActiveUntilState);
	const bool bTooLate = (NewState > ActiveUntilState);

	if (bTooLate)
	{
		if (PathEffect)
		{
			PathEffect->Deactivate();
			PathEffect->DestroyComponent();
		}
		if (BlockingBox)
		{
			BlockingBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		if (CollisionBox)
		{
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		// 문은 존재
		if (PathEffect && !PathEffect->IsActive())
		{
			PathEffect->Activate();
		}
		if (BlockingBox)
		{
			BlockingBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

		if (bValid)
		{
			if (CollisionBox)
			{
				CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
		else if (bTooEarly)
		{
			if (CollisionBox)
			{
				CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ABossCloudDoor::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABossCloudDoor::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABossCloudDoor::OnOverlapEnd);

	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->OnStoryStateChanged.AddDynamic(this, &ABossCloudDoor::OnStoryStateUpdated);
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
}

void ABossCloudDoor::EnterDoor()
{
	if (!CachedCharacter || !EnterDoorMontage)
		return;

	UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;
	
	GetWorld()->GetTimerManager().SetTimer(WalkMontageTimerHandle, this, &ABossCloudDoor::PlayWalkMontageLoop, 0.1f, true);

	//AnimInstance->Montage_Play(EnterDoorMontage);
}

void ABossCloudDoor::PlayWalkMontageLoop()
{
	if (!CachedCharacter || !EntryTeleportTarget)
		return;

	FVector Current = CachedCharacter->GetActorLocation();
	FVector Target = EntryTeleportTarget->GetComponentLocation();
	Target.Z = Current.Z;

	const float Distance = FVector::Dist(Current, Target);
	if (Distance > 30.f)
	{
		UAnimInstance* AnimInstance = CachedCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(EnterDoorMontage))
		{
			AnimInstance->Montage_Play(EnterDoorMontage);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(WalkMontageTimerHandle);
		OnEnterMontageEnded(EnterDoorMontage, false);
	}
}

void ABossCloudDoor::OnEnterMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsMontagePlaying = false;
	if (CachedCharacter)
	{
		APlayerController* PC = Cast<APlayerController>(CachedCharacter->GetController());
		if (PC)
		{
			if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->SetPlayerInputEnable(true);
			}
		}

		BlockingBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		BlockingBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		BlockingBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));

		if (GI->GetCurrentGameStoryState() == ActiveFromState)
		{
			LinkedBossSpawner->SpawnBoss();
			GI->AdvanceStoryState();
		}
	}
	CachedCharacter = nullptr;
	Debug::Print(TEXT("몽타주 끝"));
}

void ABossCloudDoor::Interact_Implementation(AActor* Interactor)
{
	ADomiCharacter* Player = Cast<ADomiCharacter>(Interactor);
	if (!Player || !EnterDoorMontage) return;

	CachedCharacter = Player;
	bIsMontagePlaying = true;

	if (EntryTeleportTarget)
	{
		FVector Target = EntryTeleportTarget->GetComponentLocation();
		FVector CharacterL = CachedCharacter->GetActorLocation();

		FRotator LookAtRotation = (Target - CharacterL).Rotation();
		LookAtRotation.Pitch = 0.f;
		LookAtRotation.Roll = 0.f;

		CachedCharacter->SetActorRotation(LookAtRotation);
	}

	APlayerController* PC = Cast<APlayerController>(CachedCharacter->GetController());
	if (PC)
	{
		if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->SetPlayerInputEnable(false);
		}
	}
	if (BlockingBox)
	{
		BlockingBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		BlockingBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		BlockingBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	}
	
	EnterDoor();
}

void ABossCloudDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
	{
		return;
	}

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	ensure(PlayerCharacter);
	CachedCharacter = PlayerCharacter;

	PlayerCharacter->AddInteractableActor(this);
}

void ABossCloudDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);
		PlayerCharacter->RemoveInteractableActor(this);

		if (!bIsMontagePlaying)
		{
			CachedCharacter = nullptr;
		}
	}
}

FText ABossCloudDoor::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("들어가기"));
}