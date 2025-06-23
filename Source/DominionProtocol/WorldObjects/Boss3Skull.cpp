// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss3Skull.h"
#include "Components/SphereComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Player/Characters/DomiCharacter.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"

#include "Util/DebugHelper.h"

ABoss3Skull::ABoss3Skull()
	: ShakeTimeDuration(0.02f),
	  MaxShakeTime(2.f),
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
	InteractableCollisionSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractableCollisionSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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
	SkullMeshComponent->SetVisibility(false);
	SkullMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AltarMeshComponent->SetVisibility(false);
	AltarMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	InteractableCollisionSphereComponent->DestroyComponent();

	if (!BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - !BossClass || !BossTag.IsValid()"));
		return;
	}
	AActor* SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossClass, GetActorLocation(),	GetActorRotation());

	if (SpawnedBoss)
	{
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
		if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->StartBattle(SpawnedBoss);
		}
	}
}