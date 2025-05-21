// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkillComponent/Skills/SkillObject/LaserActor.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Player/Damagable.h"

// Sets default values
ALaserActor::ALaserActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TraceLength = 1500.f;
	float HalfTraceLength = TraceLength / 2;

	OwnerSkillTag = SkillTags::LaserSkill;

	// Trace Setting
	TraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("TraceComponent"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	CapsuleComponent = CreateDefaultSubobject<UMnhCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SceneComponent);
	CapsuleComponent->SetCapsuleHalfHeight(HalfTraceLength);
	CapsuleComponent->SetRelativeLocation(FVector(HalfTraceLength, 0.f, 0.f));
	CapsuleComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	FMnhTracerConfig TracerConfig;

	TracerConfig.TracerTag = OwnerSkillTag;
	TracerConfig.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;
	TracerConfig.TraceSource = EMnhTraceSource::MnhShapeComponent;

	TraceComponent->TracerConfigs.Add(TracerConfig);
	TraceComponent->FilterType = EMnhFilterType::None;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(OwnerSkillTag);

	TraceComponent->InitializeTracers(TagContainer, CapsuleComponent);

	LaserBeamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserBeamEffect"));
	LaserBeamEffect->SetupAttachment(CapsuleComponent);

	LaserFireBallEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserFireBallEffect"));
	LaserFireBallEffect->SetupAttachment(CapsuleComponent);

	LaserHitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserHitEffect"));
	LaserHitEffect->SetupAttachment(CapsuleComponent);
}

// Called when the game starts or when spawned
void ALaserActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FSkillData* SkillData = BaseGameState->GetSkillData(OwnerSkillTag))
			{
				if (SkillData->NaiagaraParticle.Num() >= 3)
				{
					LaserFireBallEffect = NewObject<UNiagaraComponent>(this);
					LaserFireBallEffect->SetAsset(SkillData->NaiagaraParticle[1]);
					LaserFireBallEffect->RegisterComponent();
					LaserFireBallEffect->AttachToComponent(
						RootComponent,
						FAttachmentTransformRules::SnapToTargetIncludingScale
					);
				}
			}
		}
	}
}

void ALaserActor::Initialize()
{
	if (IsValid(TraceComponent))
	{
		// 델리게이트 바인딩
		TraceComponent->OnHitDetected.AddDynamic(this, &ThisClass::HandleTracerHit);
	}

	check(IsValid(OwnerCharacter));
	check(OwnerSkillTag.IsValid());

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(OwnerSkillTag);

	TraceComponent->AddToIgnoredActors(TagContainer, OwnerCharacter);
	TraceComponent->StartTracers(TagContainer);

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FSkillData* SkillData = BaseGameState->GetSkillData(OwnerSkillTag))
			{
				if (SkillData->NaiagaraParticle.IsValidIndex(2))
				{
					LaserBeamEffect = NewObject<UNiagaraComponent>(this);
					LaserBeamEffect->SetAsset(SkillData->NaiagaraParticle[0]);
					LaserBeamEffect->RegisterComponent();
					LaserBeamEffect->AttachToComponent(
						RootComponent,
						FAttachmentTransformRules::SnapToTargetIncludingScale
					);

					LaserHitEffect = NewObject<UNiagaraComponent>(this);
					LaserHitEffect->SetAsset(SkillData->NaiagaraParticle[2]);
					LaserHitEffect->RegisterComponent();
					LaserHitEffect->AttachToComponent(
						RootComponent,
						FAttachmentTransformRules::SnapToTargetIncludingScale
					);
				}

				if (SkillData->SkillMaterials.IsValidIndex(1))
				{
					DecalMaterial1 = SkillData->SkillMaterials[0];
					DecalMaterial2 = SkillData->SkillMaterials[1];
				}
			}
		}
	}
}

// 레이저 이펙트 업데이트에 사용
void ALaserActor::Tick(float DeltaTime)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	if (IsValid(SkillComponent))
	{
		UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

		if (BaseSkill == nullptr || BaseSkill->GetSkillTag() != SkillTags::LaserSkill)
		{
			Destroy();
		}
	}

	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}

	check(OwnerCharacter);

	// 충돌 파라미터 설정
	FCollisionQueryParams Params;

	// 소유 캐릭터 무시
	Params.AddIgnoredActor(OwnerCharacter);

	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * TraceLength;

	FHitResult HitResult;

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	const float Distance = bHit ? HitResult.Distance : TraceLength;

	if (IsValid(LaserBeamEffect))
	{
		LaserBeamEffect->SetVariablePosition(FName("Start"), Start);
		LaserBeamEffect->SetVariablePosition(FName("End"), Start + GetActorForwardVector() * Distance);
	}

	if (IsValid(LaserHitEffect))
	{
		if (!bHit)
		{
			LaserHitEffect->SetVisibility(false);
		}

		LaserHitEffect->SetVisibility(true);
		LaserHitEffect->SetWorldLocation(HitResult.Location);
		LaserHitEffect->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal));
	}

	if (IsValid(DecalMaterial1))
	{
		FVector DecalSize;

		DecalSize.X = FMath::FRandRange(5.f, 20.f);
		DecalSize.Y = FMath::FRandRange(5.f, 20.f);
		DecalSize.Z = FMath::FRandRange(5.f, 20.f);

		// 데칼 생성
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			DecalMaterial1,
			DecalSize,
			HitResult.Location,
			FRotator(0.0f, 0.0f, -90.0f),
			2.f
		);
	}

	if (IsValid(DecalMaterial2))
	{
		FVector DecalSize;

		DecalSize.X = FMath::FRandRange(10.f, 40.f);
		DecalSize.Y = FMath::FRandRange(10.f, 40.f);
		DecalSize.Z = FMath::FRandRange(10.f, 40.f);

		// 데칼 생성
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			DecalMaterial2,
			DecalSize,
			HitResult.Location,
			FRotator(0.0f, 0.0f, -90.0f),
			4.f
		);
	}
}

void ALaserActor::SetOwnerCharacter(AActor* NewOwnerCharacter)
{
	OwnerCharacter = NewOwnerCharacter;
}

void ALaserActor::HandleTracerHit(FGameplayTag TracerTag, FHitResult PrimaryHitResult, float DeltaTime)
{
	AActor* PrimaryHitActor = PrimaryHitResult.GetActor();

	if (!IsValid(PrimaryHitActor))
	{
		return;
	}

	if (!(PrimaryHitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass())))
	{
		return;
	}

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}

	check(OwnerCharacter);

	// 충돌 파라미터 설정
	FCollisionQueryParams Params;

	// 소유 캐릭터 무시
	Params.AddIgnoredActor(OwnerCharacter);
	Params.AddIgnoredActor(this);

	const FVector Start = GetActorLocation();
	const FVector End = PrimaryHitResult.ImpactPoint;

	FHitResult SecondaryHitResult;

	bool bHit = World->LineTraceSingleByChannel(
		SecondaryHitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	AActor* SecondaryActor = SecondaryHitResult.GetActor();

	if (IsValid(SecondaryActor) && SecondaryActor != PrimaryHitActor)
	{
		return;
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	if (IsValid(SkillComponent))
	{
		UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

		if (IsValid(BaseSkill))
		{
			if (BaseSkill->GetSkillTag() == SkillTags::LaserSkill)
			{
				BaseSkill->ApplyAttackToHitActor(PrimaryHitResult, DeltaTime);
			}
		}
	}
}