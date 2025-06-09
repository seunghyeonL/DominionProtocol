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

	BlockingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Blocking"));
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
	Debug::Print(FString::Printf(TEXT("BossCloudDoor: OnStoryStateUpdated - CurrentState = %d, From = %d, Until = %d"),
		static_cast<int32>(NewState),
		static_cast<int32>(ActiveFromState),
		static_cast<int32>(ActiveUntilState)));

	const bool bTooEarly = (NewState < ActiveFromState);
	const bool bValid = (NewState >= ActiveFromState && NewState <= ActiveUntilState);
	const bool bTooLate = (NewState > ActiveUntilState);

	if (bTooLate)
	{
		Debug::Print(TEXT("BossCloudDoor: Too late → Remove door"));

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
			Debug::Print(TEXT("BossCloudDoor: Valid → 상호작용 가능"));
			if (CollisionBox)
			{
				CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
		else if (bTooEarly)
		{
			Debug::Print(TEXT("BossCloudDoor: Too early → 상호작용 불가"));
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
	float Duration = AnimInstance->Montage_Play(EnterDoorMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABossCloudDoor::OnEnterMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EnterDoorMontage);
}

void ABossCloudDoor::OnEnterMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Debug::Print(TEXT("ABossCloudDoor: OnEnterMontageEnded"));
	bIsMontagePlaying = false;
	if (CachedCharacter)
	{
		APlayerController* PC = Cast<APlayerController>(CachedCharacter->GetController());
		if (PC)
		{
			if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->SetPlayerInputEnable(true);
				Debug::Print(TEXT("ABossCloudDoor: SetPlayerInputEnable true"));
			}
			else
			{
				Debug::Print(TEXT("ABossCloudDoor: ABaseGameMode NotValid"));
			}
		}
		else
		{
			Debug::Print(TEXT("ABossCloudDoor: APlayerController NotValid"));
		}

		//PathEffect->Activate();
		//BlockingBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BlockingBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		BlockingBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		BlockingBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

		if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			GI->AdvanceStoryState();
			Debug::Print(TEXT("ABossCloudDoor: 스토리 상태 변경"));
		}

		Debug::Print(TEXT("ABossCloudDoor: Calling SpawnBoss()"));
		LinkedBossSpawner->SpawnBoss();

	}
	else
	{
		Debug::Print(TEXT("ABossCloudDoor: CachedCharacter NotValid"));
	}
	CachedCharacter = nullptr;
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

		CachedCharacter->SetActorRotation(LookAtRotation);
	}

	APlayerController* PC = Cast<APlayerController>(CachedCharacter->GetController());
	if (PC)
	{
		if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->SetPlayerInputEnable(false);
			Debug::Print(TEXT("ABossCloudDoor: SetPlayerInputEnable false"));
		}
	}

	//BlockingBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlockingBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	BlockingBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BlockingBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	EnterDoor();
}

void ABossCloudDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("ABossCloudDoor: Overlap"));

	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("ABossCloudDoor: Not Player"));
		return;
	}

	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI)
	{
		Debug::Print(TEXT("ABossCloudDoor: GameInstance Not Valid"));
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
	else
	{
		Debug::Print(TEXT("ABossCloudDoor::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}

FText ABossCloudDoor::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("들어가기"));
}